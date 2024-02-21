// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverwatchGameMode.h"
#include "OverwatchCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOverwatchGameMode::AOverwatchGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
