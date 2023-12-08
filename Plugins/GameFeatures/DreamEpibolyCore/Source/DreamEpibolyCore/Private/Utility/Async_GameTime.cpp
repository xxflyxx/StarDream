// Fill out your copyright notice in the Description page of Project Settings.


#include "Async_GameTime.h"
#include "GamePhase/MessageGamePhase.h"

UAsync_GameTime* UAsync_GameTime::AsyncGameTime(AActor* TargetActor, float Delay, float Mod)
{
	check(TargetActor);
	
	UAsync_GameTime* MyObj = nullptr;
	if (TargetActor)
	{
		MyObj = NewObject<UAsync_GameTime>();
		MyObj->World = TargetActor->GetWorld();
		MyObj->Delay = Delay;
		MyObj->Mod = Mod;
	}

	return MyObj;
}

void UAsync_GameTime::Activate()
{
	Super::Activate();

	MyHandle = UGameplayMessageSubsystem::Get(World).RegisterListener<FMessageGameTime>(Tag_Message_GameTime, this, &UAsync_GameTime::OnGameTime);
}

void UAsync_GameTime::Cancel()
{
	MyHandle.Unregister();
	Super::Cancel();
}


void UAsync_GameTime::OnGameTime(FGameplayTag, const FMessageGameTime& MessageGameTime)
{
	const float ModTime = Mod>0.f ? FMath::Fmod(MessageGameTime.SinceGameStart, Mod) : 0.f;
	OnTick.Broadcast(MessageGameTime.SinceGameStart, MessageGameTime.DeltaTime, ModTime);
}
