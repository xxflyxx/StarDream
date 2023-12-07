// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MessageGamePhase.h"
#include "Components/ActorComponent.h"
#include "Components/GameStateComponent.h"
#include "GamePhaseComponent.generated.h"

USTRUCT(BlueprintType)
struct FPhaseWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EGamePhase Phase = EGamePhase::Waiting;
	UPROPERTY(EditAnywhere)
	FGameplayTag SlotTag;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> Widget;
	UPROPERTY(EditAnywhere)
	int Priority = 1;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREAMEPIBOLYCORE_API UGamePhaseComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	UGamePhaseComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	float GetServerTimeSeconds() const;
	UFUNCTION(BlueprintCallable)
	float GetTimeSecondsSinceGameStart() const;

	UPROPERTY(EditAnywhere)
	TArray<FPhaseWidget> PhaseWidgets;
protected:
	virtual void BeginPlay() override;
	void ChangePhase(EGamePhase NewPhase);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION()
	void OnRep_GameStartTime();
	
	int MinGamePlayerNum = 1;
	float GameLife = 60.f;
	float GameReadyTime = 10.f;
	EGamePhase CurrentPhase = EGamePhase::Waiting;

	// GameStartTime 服务端计算并复制下来，保证精确
	UPROPERTY(ReplicatedUsing=OnRep_GameStartTime)
	float GameStartTime = 0.f;
};
