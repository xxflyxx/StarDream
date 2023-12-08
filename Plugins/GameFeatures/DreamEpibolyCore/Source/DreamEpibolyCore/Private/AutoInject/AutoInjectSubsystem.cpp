// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoInjectSubsystem.h"

#include "Components/GameFrameworkComponentManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameStateBase.h"

void UAutoInjectSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	GetWorld()->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateWeakLambda(this, [this](AActor* SpawnedActor)
	{
		if (!GameFrameworkComponentManager)
		{
			UGameInstance* GameInstance = GetWorld()->GetGameInstance();
			if (!GameInstance)
				return;
			GameFrameworkComponentManager = GameInstance->GetSubsystem<UGameFrameworkComponentManager>();
			check(GameFrameworkComponentManager);
		}
		static TArray<UClass*> Classes =
		{
			ACharacter::StaticClass(),
			AGameStateBase::StaticClass(),
			AGameModeBase::StaticClass(),
			AController::StaticClass()
		};
		for (UClass* Class : Classes)
		{
			if (SpawnedActor->IsA(Class))
			{
				GameFrameworkComponentManager->AddReceiver(SpawnedActor);
				SpawnedActor->OnEndPlay.AddDynamic(this, &ThisClass::OnActorEndPlay);
				break;
			}
		}
	}));
}

void UAutoInjectSubsystem::OnActorEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	GameFrameworkComponentManager->RemoveReceiver(Actor);
}
