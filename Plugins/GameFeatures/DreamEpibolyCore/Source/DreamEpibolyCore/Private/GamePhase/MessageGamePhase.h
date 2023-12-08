
#pragma once

#include "CoreMinimal.h"
// #include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "MessageGamePhase.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Message_ServerTime);
USTRUCT(BlueprintType)
struct FMessageServerTime
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float ServerTimeSeconds = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float DeltaTime = 0.f;

	FMessageServerTime() {}
	FMessageServerTime(float ServerTIme, float Delta) : ServerTimeSeconds(ServerTIme), DeltaTime(Delta) {}
};

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Message_GameTime);
USTRUCT(BlueprintType)
struct FMessageGameTime
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float SinceGameStart = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float DeltaTime = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float RemainTime = 0.f;

	FMessageGameTime() {}
	FMessageGameTime(float GameTime, float Delta, float Remain) : SinceGameStart(GameTime), DeltaTime(Delta), RemainTime(Remain) {}
};

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	Waiting,
	Ready,
	Start,
	GameOver
};

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Message_GamePhase);
USTRUCT(BlueprintType)
struct FMessageGamePhase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	EGamePhase Phase = EGamePhase::Waiting;

	FMessageGamePhase() {}
	FMessageGamePhase(EGamePhase InPhase) : Phase(InPhase) {}
};

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Message_PlayerArrived);
USTRUCT(BlueprintType)
struct FMessagePlayerArrived
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	AActor* Player = nullptr;

	FMessagePlayerArrived() {}
	FMessagePlayerArrived(AActor* InPlayer) : Player(InPlayer) {}
};
