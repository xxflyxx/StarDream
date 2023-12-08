// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ConveyorComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREAMEPIBOLYCORE_API UConveyorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UConveyorComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	float Speed = 100.f;
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnOverlapChange(AActor* OverlappedActor, AActor* OtherActor);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
