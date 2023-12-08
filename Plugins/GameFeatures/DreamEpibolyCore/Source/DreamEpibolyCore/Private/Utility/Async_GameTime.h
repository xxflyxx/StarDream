// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CancellableAsyncAction.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GamePhase/MessageGamePhase.h"
#include "Async_GameTime.generated.h"

/**
 * 
 */
UCLASS()
class DREAMEPIBOLYCORE_API UAsync_GameTime : public UCancellableAsyncAction
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE"))
	static UAsync_GameTime* AsyncGameTime(AActor* TargetActor, float Delay = 0.f, float Mod = 0.f);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAsyncGameTime, float, Time, float, DeltaTime, float, ModTime);
	UPROPERTY(BlueprintAssignable)
	FAsyncGameTime OnTick;
protected:
	virtual void Activate() override;
	virtual void Cancel() override;
	void OnGameTime(FGameplayTag TagChannel, const FMessageGameTime& MessageGameTime);

	UPROPERTY()
	class UWorld* World = nullptr;
	float Delay = 0.f;
	float Mod = 0.f;

	FGameplayMessageListenerHandle MyHandle;
};
