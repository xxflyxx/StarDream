// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorComponent.h"

#include "Components/ShapeComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UConveyorComponent::UConveyorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UConveyorComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
	GetOwner()->OnActorBeginOverlap.AddDynamic(this, &UConveyorComponent::OnOverlapChange);
	GetOwner()->OnActorEndOverlap.AddDynamic(this, &UConveyorComponent::OnOverlapChange);
}

void UConveyorComponent::OnOverlapChange(AActor* OverlappedActor, AActor* OtherActor)
{
	TArray<AActor*> OverlappingActors;
	GetOwner()->GetOverlappingActors(OverlappingActors);
	SetComponentTickEnabled(OverlappingActors.Num() > 0);
}

void UConveyorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<UShapeComponent*> OverlapComponents;
	GetOwner()->GetComponents<UShapeComponent>(OverlapComponents);
	for (UShapeComponent* OverlapComponent : OverlapComponents)
	{
		TArray<AActor*> OverlapActors;
		OverlapComponent->GetOverlappingActors(OverlapActors);
		for (AActor* OverlapActor : OverlapActors)
		{
			// if (auto Character = Cast<ACharacter>(OverlapActor))
			// {
			// 	Character->GetCharacterMovement()->RequestDirectMove(OverlapComponent->GetComponentRotation().Vector() * Speed, false);
			// }
			FVector OffsetLoc = OverlapComponent->GetComponentRotation().Vector() * Speed * DeltaTime;
			OverlapActor->AddActorWorldOffset(OffsetLoc, true, nullptr, ETeleportType::TeleportPhysics);
		}
	}
}


