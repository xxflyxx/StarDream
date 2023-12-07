// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "Checkpoint.generated.h"

UCLASS(Abstract, Blueprintable)
class DREAMEPIBOLYCORE_API ACheckpoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	ACheckpoint();
	FTransform GetRebornTransform() const;
	UNiagaraSystem* GetSpawnEffect() const { return SpawnEffect; }
protected:

	UPROPERTY(EditAnywhere)
	float RebornRandRadius = 300.0f;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* SpawnEffect = nullptr;
};
