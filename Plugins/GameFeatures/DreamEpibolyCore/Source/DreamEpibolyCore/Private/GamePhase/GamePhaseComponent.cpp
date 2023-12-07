// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePhaseComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

UE_DEFINE_GAMEPLAY_TAG(Message_ServerTime, "Message.ServerTime");
UE_DEFINE_GAMEPLAY_TAG(Message_GameTime, "Message.GameTime");
UE_DEFINE_GAMEPLAY_TAG(Message_GamePhase, "Message.GamePhase");

UE_DEFINE_GAMEPLAY_TAG(UI_GameTime, "UI.GameTime");

UGamePhaseComponent::UGamePhaseComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bRegistered = true;
	PrimaryComponentTick.bCanEverTick = true;
	
}

float UGamePhaseComponent::GetServerTimeSeconds() const
{
	AGameStateBase* GameState = GetOwner<AGameStateBase>();
	return GameState->GetServerWorldTimeSeconds();
}


float UGamePhaseComponent::GetTimeSecondsSinceGameStart() const
{
	AGameStateBase* GameState = GetOwner<AGameStateBase>();
	const float ServerTime = GameState->GetServerWorldTimeSeconds();
	return ServerTime-GameStartTime;
}

// Called when the game starts
void UGamePhaseComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("UGamePhaseComponent::BeginPlay"));
	AGameStateBase* GameState = GetOwner<AGameStateBase>();
	check(GameState);
	if (!IsNetMode(NM_Client))
	{
		const FString& OptionsString = GameState->AuthorityGameMode->OptionsString;
		
		MinGamePlayerNum = UGameplayStatics::GetIntOption(OptionsString, TEXT("MinPlayerNum"), 1);
		const FString GameLifeString = UGameplayStatics::ParseOption(OptionsString, TEXT("GameLife"));
		if (GameLifeString.Len()>0)
		{
			GameLife = FCString::Atof(*GameLifeString);
		}
	}
		
}

void UGamePhaseComponent::ChangePhase(EGamePhase NewPhase)
{
	if (NewPhase!= CurrentPhase)
	{
		CurrentPhase = NewPhase;
		UGameplayMessageSubsystem::Get(GetWorld()).BroadcastMessage(Message_GamePhase, FGamePhaseMessage(CurrentPhase));
	}
}


// Called every frame
void UGamePhaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AGameStateBase* GameState = GetOwner<AGameStateBase>();
	const float ServerTime = GameState->GetServerWorldTimeSeconds();
	UGameplayMessageSubsystem::Get(GetWorld()).BroadcastMessage(Message_ServerTime, FServerTimeMessage(ServerTime, DeltaTime));

	if (CurrentPhase == EGamePhase::Waiting)
	{
		// GameStartTime 服务端计算并复制下来，保证精确
		if (!IsNetMode(NM_Client))
		{
			if (GameState->PlayerArray.Num()>MinGamePlayerNum)
			{
				GameStartTime = ServerTime + GameReadyTime;
				ChangePhase(EGamePhase::Ready);
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
			UGameplayMessageSubsystem::Get(GetWorld()).BroadcastMessage(Message_GameTime, FGameTimeMessage(ServerTime-GameStartTime, DeltaTime, RemainTime));
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

