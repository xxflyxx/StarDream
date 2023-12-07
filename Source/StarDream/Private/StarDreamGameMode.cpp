// Copyright Epic Games, Inc. All Rights Reserved.

#include "StarDreamGameMode.h"
#include "StarDreamCharacter.h"
#include "StarDreamGameState.h"
#include "UObject/ConstructorHelpers.h"

AStarDreamGameMode::AStarDreamGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	GameStateClass = AStarDreamGameState::StaticClass();
}
