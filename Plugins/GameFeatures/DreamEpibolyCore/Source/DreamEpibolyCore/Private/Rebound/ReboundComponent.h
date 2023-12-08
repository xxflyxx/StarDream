// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReboundComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREAMEPIBOLYCORE_API UReboundComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UReboundComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	float Impulse = 800.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	class UNiagaraSystem* ImpulseEffect = nullptr;
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
