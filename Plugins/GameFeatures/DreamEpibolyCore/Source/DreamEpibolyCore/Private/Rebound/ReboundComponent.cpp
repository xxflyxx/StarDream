// Fill out your copyright notice in the Description page of Project Settings.


#include "ReboundComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UReboundComponent::UReboundComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CPF_Config
}

void UReboundComponent::BeginPlay()
{
	Super::BeginPlay();
	TArray<UPrimitiveComponent*> Components;
	GetOwner()->GetComponents<UPrimitiveComponent>(Components);
	for (UPrimitiveComponent* Component : Components)
	{
		if (!Component->IsCollisionEnabled())
			return;
		Component->OnComponentHit.AddDynamic(this, &UReboundComponent::OnHit);
	}
}

void UReboundComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		const FVector Direction = -Hit.ImpactNormal.GetSafeNormal();
		Character->LaunchCharacter(Direction*Impulse, true, true);
		// Character->GetCharacterMovement()->AddImpulse(Direction*Impulse, true);

		if (ImpulseEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpulseEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}
	}
}





