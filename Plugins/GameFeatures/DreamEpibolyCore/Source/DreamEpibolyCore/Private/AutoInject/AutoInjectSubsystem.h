// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AutoInjectSubsystem.generated.h"

/**
 * 自动注入game feature相关内容
 */
UCLASS()
class DREAMEPIBOLYCORE_API UAutoInjectSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION()
	void OnActorEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason);

	UPROPERTY()
	class UGameFrameworkComponentManager* GameFrameworkComponentManager = nullptr;
};
