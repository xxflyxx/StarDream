// Fill out your copyright notice in the Description page of Project Settings.


#include "StarDreamGameState.h"

#include "Components/GameFrameworkComponentManager.h"


// Sets default values
AStarDreamGameState::AStarDreamGameState()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AStarDreamGameState::BeginPlay()
{
	Super::BeginPlay();

	GetGameInstance()->GetSubsystem<UGameFrameworkComponentManager>()->AddReceiver(this);
}

void AStarDreamGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetGameInstance()->GetSubsystem<UGameFrameworkComponentManager>()->RemoveReceiver(this);
	
	Super::EndPlay(EndPlayReason);
}


void AStarDreamGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	// if (GetWorld())
	// 	GetGameInstance()->GetSubsystem<UGameFrameworkComponentManager>()->AddReceiver(this);
}
