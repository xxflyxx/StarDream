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
	
}


void AStarDreamGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (GetWorld())
		GetGameInstance()->GetSubsystem<UGameFrameworkComponentManager>()->AddReceiver(this);
}
