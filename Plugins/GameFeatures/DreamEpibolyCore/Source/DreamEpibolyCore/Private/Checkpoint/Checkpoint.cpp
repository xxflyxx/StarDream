// Fill out your copyright notice in the Description page of Project Settings.

#include "Checkpoint.h"

ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent->SetMobility(EComponentMobility::Static);
}

FTransform ACheckpoint::GetRebornTransform() const
{
	FTransform Transform = GetActorTransform();
	const FVector2D V = FMath::RandPointInCircle(RebornRandRadius);
	Transform.SetLocation(Transform.GetLocation() + FVector(V, 0.f));
	Transform.SetScale3D(FVector::OneVector);
	return Transform;
}


