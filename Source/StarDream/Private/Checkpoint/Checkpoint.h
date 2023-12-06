// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

UCLASS()
class STARDREAM_API ACheckpoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	ACheckpoint();
	FTransform GetRebornTransform() const;
protected:

	UPROPERTY(EditAnywhere)
	float RebornRandRadius = 300.0f;

};
