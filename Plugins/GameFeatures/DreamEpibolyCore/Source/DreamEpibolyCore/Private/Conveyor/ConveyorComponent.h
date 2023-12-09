// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ConveyorComponent.generated.h"

USTRUCT()
struct FComponentRootSourceID
{
	GENERATED_BODY()
		
	UPROPERTY()
	TMap<AActor*, uint16> MapSourceID;
};

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
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
	TMap<UPrimitiveComponent*, FComponentRootSourceID> ComponentRootSourceIDs;
};
