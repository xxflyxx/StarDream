// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePhaseComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_Message_ServerTime, "Message.ServerTime");
UE_DEFINE_GAMEPLAY_TAG(Tag_Message_GameTime, "Message.GameTime");
UE_DEFINE_GAMEPLAY_TAG(Tag_Message_GamePhase, "Message.GamePhase");
UE_DEFINE_GAMEPLAY_TAG(Tag_Message_PlayerArrived, "Message.PlayerArrived");

UGamePhaseComponent::UGamePhaseComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bCanEverTick = true;
}

float UGamePhaseComponent::GetServerTimeSeconds() const
{
	return GetOwner<AGameStateBase>()->GetServerWorldTimeSeconds();
}

float UGamePhaseComponent::GetTimeSecondsSinceGameStart() const
{
	return GetServerTimeSeconds()-GameStartTime;
}

float UGamePhaseComponent::GetMinPlayerNum() const
{
	return MinGamePlayerNum;
}

// Called when the game starts
void UGamePhaseComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("UGamePhaseComponent::BeginPlay"));
	AGameStateBase* GameState = GetOwner<AGameStateBase>();
	check(GameState);
	if (!IsNetMode(NM_Client))
	{
		const FString& OptionsString = GameState->AuthorityGameMode->OptionsString;
		
		MinGamePlayerNum = UGameplayStatics::GetIntOption(OptionsString, TEXT("MinPlayerNum"), MinGamePlayerNum);
		const FString GameLifeString = UGameplayStatics::ParseOption(OptionsString, TEXT("GameLife"));
		if (GameLifeString.Len()>0)
		{
			GameLife = FCString::Atof(*GameLifeString);
		}
	}
	
	if (!IsNetMode(NM_DedicatedServer))
	{
		UUserWidget* Widget = CreateWidget(GetWorld(), LayoutWidget);
		Widget->AddToViewport();

		CreatePhaseWidgets();
	}
}

void UGamePhaseComponent::ChangePhase(EGamePhase NewPhase)
{
	if (NewPhase!= CurrentPhase)
	{
		CurrentPhase = NewPhase;
		UE_LOG(LogTemp, Log, TEXT("ChangePhase %d"), NewPhase);
		CreatePhaseWidgets();
		UGameplayMessageSubsystem::Get(GetWorld()).BroadcastMessage(Tag_Message_GamePhase, FMessageGamePhase(CurrentPhase));
	}
}

void UGamePhaseComponent::CreatePhaseWidgets()
{
	if (IsNetMode(NM_DedicatedServer))
		return;

	for (FUIExtensionHandle& Handle : PhaseWidgetHandles)
	{
		Handle.Unregister();
	}
	PhaseWidgetHandles.Reset();
	
	for (const FPhaseWidget& WidgetInfo : PhaseWidgets)
	{
		if (WidgetInfo.Phase == CurrentPhase)
		{
			PhaseWidgetHandles.Add(GetWorld()->GetSubsystem<UUIExtensionSubsystem>()->RegisterExtensionAsWidget(WidgetInfo.SlotTag, WidgetInfo.Widget, WidgetInfo.Priority));
		}
	}
}


void UGamePhaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AGameStateBase* GameState = GetOwner<AGameStateBase>();
	const float ServerTime = GameState->GetServerWorldTimeSeconds();
	UGameplayMessageSubsystem::Get(GetWorld()).BroadcastMessage(Tag_Message_ServerTime, FMessageServerTime(ServerTime, DeltaTime));

	if (CurrentPhase == EGamePhase::Waiting)
	{
		// GameStartTime 服务端计算并复制下来，保证精确
		if (!IsNetMode(NM_Client))
		{
			if (GameState->PlayerArray.Num()>=MinGamePlayerNum)
			{
				GameStartTime = ServerTime + GameReadyTime;
				OnRep_GameStartTime();
			}
		}
	}
	else if (CurrentPhase == EGamePhase::Ready)
	{
		if (ServerTime>GameStartTime)
		{
			ChangePhase(EGamePhase::Start);
		}
	}
	else if (CurrentPhase == EGamePhase::Start)
	{
		const float RemainTime = GameStartTime+GameLife-ServerTime;
		if (RemainTime<0.f)
		{
			ChangePhase(EGamePhase::GameOver);
		}
		else
		{
			UGameplayMessageSubsystem::Get(GetWorld()).BroadcastMessage(Tag_Message_GameTime, FMessageGameTime(ServerTime-GameStartTime, DeltaTime, RemainTime));
		}
	}
}

void UGamePhaseComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, GameStartTime);
}

void UGamePhaseComponent::OnRep_GameStartTime()
{
	if (GameStartTime > 0.f)
		ChangePhase(EGamePhase::Ready);
}

