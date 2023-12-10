// Fill out your copyright notice in the Description page of Project Settings.


#include "FanComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"


FName UFanComponent::IgnoreFan = FName("Ignore_Fan");

UFanComponent::UFanComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;
}

void UFanComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);

	TArray<UBoxComponent*> OverlapComponents;
	GetOwner()->GetComponents<UBoxComponent>(OverlapComponents);
	for (UBoxComponent* OverlapComponent : OverlapComponents)
	{
		if (OverlapComponent->ComponentHasTag(IgnoreFan))
			continue;
		
		if (OverlapComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly)
		{
			OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapChange);
		}
	}
}

void UFanComponent::OnOverlapChange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const bool bIsTickEnable = IsComponentTickEnabled();
	TArray<UBoxComponent*> OverlapComponents;
	GetOwner()->GetComponents<UBoxComponent>(OverlapComponents);
	for (UBoxComponent* OverlapComponent : OverlapComponents)
	{
		if (OverlapComponent->ComponentHasTag(IgnoreFan))
			continue;
		
		if (OverlapComponent->GetOverlapInfos().Num() > 0)
		{
			if (!bIsTickEnable)
				SetComponentTickEnabled(true);
			return;
		}
	}

	if (bIsTickEnable)
		SetComponentTickEnabled(false);
}


void UFanComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<UBoxComponent*> OverlapComponents;
	GetOwner()->GetComponents<UBoxComponent>(OverlapComponents);
	for (UBoxComponent* OverlapComponent : OverlapComponents)
	{
		if (OverlapComponent->ComponentHasTag(IgnoreFan))
			continue;
		
		TArray<AActor*> OverlapActors;
		OverlapComponent->GetOverlappingActors(OverlapActors);
		for (AActor* OverlapActor : OverlapActors)
		{
			if (ACharacter* Character = Cast<ACharacter>(OverlapActor))
			{
				const float Distance = (Character->GetActorLocation()-GetOwner()->GetActorLocation()).Size();
				
				const float Amplitude = FMath::GetMappedRangeValueClamped(
					FVector2D(0.f, OverlapComponent->GetScaledBoxExtent().Z),
					FVector2D(Strength, 0.f), Distance);

				FVector LaunchVelocity = OverlapComponent->GetUpVector()*Amplitude + FVector(0.f, 0.f, 20.f);
				Character->LaunchCharacter(LaunchVelocity, false, true);
			}
		}
	}
}

