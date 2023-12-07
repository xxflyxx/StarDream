
#pragma once

#include "CoreMinimal.h"
// #include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "GamePhaseMessage.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Message_ServerTime);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Message_GameTime);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Message_GamePhase);

USTRUCT(BlueprintType)
struct FServerTimeMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float ServerTimeSeconds = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float DeltaTime = 0.f;

	FServerTimeMessage() {}
	FServerTimeMessage(float ServerTIme, float Delta) : ServerTimeSeconds(ServerTIme), DeltaTime(Delta) {}
};

USTRUCT(BlueprintType)
struct FGameTimeMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float SinceGameStart = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float DeltaTime = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float RemainTime = 0.f;

	FGameTimeMessage() {}
	FGameTimeMessage(float GameTime, float Delta, float Remain) : SinceGameStart(GameTime), DeltaTime(Delta), RemainTime(Remain) {}
};

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	Waiting,
	Ready,
	Start,
	GameOver
};

USTRUCT(BlueprintType)
struct FGamePhaseMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	EGamePhase Phase = EGamePhase::Waiting;

	FGamePhaseMessage() {}
	FGamePhaseMessage(EGamePhase InPhase) : Phase(InPhase) {}
};
