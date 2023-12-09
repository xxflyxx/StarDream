// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorComponent.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UConveyorComponent::UConveyorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UConveyorComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);

	TArray<UShapeComponent*> OverlapComponents;
	GetOwner()->GetComponents<UShapeComponent>(OverlapComponents);
	for (UShapeComponent* OverlapComponent : OverlapComponents)
	{
		if (OverlapComponent->ComponentHasTag(FName(TEXT("Ignore_Conveyor"))))
			continue;
		
		if (OverlapComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly)
		{
			OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
			OverlapComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnOverlapEnd);
		}
	}
}

void UConveyorComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		TSharedPtr<FRootMotionSource_ConstantForce> ConstantForce = MakeShared<FRootMotionSource_ConstantForce>();
		ConstantForce->InstanceName = FName("Conveyor");
		ConstantForce->AccumulateMode = ERootMotionAccumulateMode::Additive;
		ConstantForce->Priority = 5;
		ConstantForce->Force = OverlappedComponent->GetComponentRotation().Vector() * Speed;
		ConstantForce->Duration = -1.f;
		uint16 RootMotionSourceID = Character->GetCharacterMovement()->ApplyRootMotionSource(ConstantForce);
		ComponentRootSourceIDs.FindOrAdd(OverlappedComponent).MapSourceID.Add(OtherActor, RootMotionSourceID);
	}
}

void UConveyorComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		uint16 RootMotionSourceID;
		ComponentRootSourceIDs.FindOrAdd(OverlappedComponent).MapSourceID.RemoveAndCopyValue(OtherActor, RootMotionSourceID);
		Character->GetCharacterMovement()->RemoveRootMotionSourceByID(RootMotionSourceID);
	}
}


