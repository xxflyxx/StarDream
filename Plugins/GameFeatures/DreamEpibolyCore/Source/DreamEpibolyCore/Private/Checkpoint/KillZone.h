// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.h"
#include "KillZone.generated.h"

UCLASS(Abstract, Blueprintable)
class DREAMEPIBOLYCORE_API AKillZone : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKillZone();

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	ACheckpoint* Checkpoint = nullptr;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Box = nullptr;

};
