// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StarDreamGameState.generated.h"

UCLASS()
class STARDREAM_API AStarDreamGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AStarDreamGameState();

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
