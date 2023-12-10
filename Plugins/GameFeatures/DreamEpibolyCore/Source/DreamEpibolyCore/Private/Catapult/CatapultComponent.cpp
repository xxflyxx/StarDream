// Fill out your copyright notice in the Description page of Project Settings.


#include "CatapultComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/Character.h"


UCatapultComponent::UCatapultComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCatapultComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!IsNetMode(NM_Client))
	{
		TArray<UShapeComponent*> OverlapComponents;
		GetOwner()->GetComponents<UShapeComponent>(OverlapComponents);
		for (UShapeComponent* OverlapComponent : OverlapComponents)
		{
			if (OverlapComponent->ComponentHasTag(FName(TEXT("Ignore_Catapult"))))
				continue;
			
			if (OverlapComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly)
			{
				OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
			}
		}
	}
}

void UCatapultComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	const float CurTime = GetWorld()->GetTimeSeconds();
	if (CurTime < NextTriggerTime)
	{
		if (!TriggerTimer.IsValid())
			GetWorld()->GetTimerManager().SetTimer(TriggerTimer, this, &ThisClass::Trigger, NextTriggerTime - CurTime, false);
		return;
	}
	Trigger();
}

void UCatapultComponent::Trigger()
{
	TriggerTimer.Invalidate();
	NextTriggerTime = GetWorld()->GetTimeSeconds() + Cooldown;
	bool bTrigger = false;
	TArray<UShapeComponent*> OverlapComponents;
	GetOwner()->GetComponents<UShapeComponent>(OverlapComponents);
	for (UShapeComponent* OverlapComponent : OverlapComponents)
	{
		TArray<AActor*> OverlapActors;
		OverlapComponent->GetOverlappingActors(OverlapActors);
		for (AActor* OverlapActor : OverlapActors)
		{
			if (ACharacter* Character = Cast<ACharacter>(OverlapActor))
			{
				bTrigger = true;
				const FVector Direction = GetComponentRotation().Vector();
				Character->LaunchCharacter(Direction*Strength, true, true);

				if (ImpulseEffect)
				{
					UNiagaraFunctionLibrary::SpawnSystemAttached(ImpulseEffect, Character->GetRootComponent(), NAME_None,
						Character->GetActorLocation(), GetComponentRotation(), EAttachLocation::KeepWorldPosition, true);
				}
			}
		}
	}
	if (bTrigger)
		OnTrigger.Broadcast();
}
