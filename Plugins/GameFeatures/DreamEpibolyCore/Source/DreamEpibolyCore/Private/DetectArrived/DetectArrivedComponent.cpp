// Fill out your copyright notice in the Description page of Project Settings.


#include "DetectArrivedComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GamePhase/MessageGamePhase.h"
#include "GameFramework/Character.h"

UDetectArrivedComponent::UDetectArrivedComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDetectArrivedComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> OverlapComponents;
	GetOwner()->GetComponents<UShapeComponent>(OverlapComponents);
	for (UShapeComponent* OverlapComponent : OverlapComponents)
	{
		if (OverlapComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly)
			OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &UDetectArrivedComponent::OnOverlapBegin);
	}
}

void UDetectArrivedComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA<ACharacter>())
	{
		bool bIsAlreadyInSetPtr = false;
		ArrivedActors.Add(OtherActor, &bIsAlreadyInSetPtr);
		if (!bIsAlreadyInSetPtr)
		{
			if (ArrivedEffect)
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(ArrivedEffect, OtherActor->GetRootComponent(), NAME_None,
					FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
			}
			
			UGameplayMessageSubsystem::Get(GetWorld()).BroadcastMessage(Tag_Message_PlayerArrived, FMessagePlayerArrived(OtherActor));	
		}
	}
}


