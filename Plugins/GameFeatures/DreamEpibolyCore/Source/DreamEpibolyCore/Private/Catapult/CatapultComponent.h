// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "CatapultComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREAMEPIBOLYCORE_API UCatapultComponent : public UArrowComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCatapultComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* ImpulseEffect = nullptr;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCatapultTrigger);
	UPROPERTY(BlueprintAssignable)
	FCatapultTrigger OnTrigger;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);
	void Trigger();

	float NextTriggerTime = 0.f;
	FTimerHandle TriggerTimer;
};
