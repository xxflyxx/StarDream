// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FanComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREAMEPIBOLYCORE_API UFanComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFanComponent();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnOverlapChange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	static FName IgnoreFan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength = 800.f;
};
