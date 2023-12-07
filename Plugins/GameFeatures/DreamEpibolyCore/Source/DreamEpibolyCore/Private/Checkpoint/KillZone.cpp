// Fill out your copyright notice in the Description page of Project Settings.


#include "KillZone.h"

#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"

AKillZone::AKillZone()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootBox"));
	Box->SetMobility(EComponentMobility::Static);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Box->SetHiddenInGame(true);
	RootComponent = Box;
}

void AKillZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (Checkpoint && OtherActor->IsA<ACharacter>())
	{
		OtherActor->SetActorTransform(Checkpoint->GetRebornTransform());
		if (UNiagaraSystem* Niagara = Checkpoint->GetSpawnEffect())
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(Niagara, OtherActor->GetRootComponent(), NAME_None,
				FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
		}
	}
}

// Called when the game starts or when spawned
void AKillZone::BeginPlay()
{
	Super::BeginPlay();
	if (Checkpoint)
		Box->OnComponentBeginOverlap.AddDynamic(this, &AKillZone::OnOverlapBegin);	
}


