// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootingRPGGameMode.h"
#include "ShootingRPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShootingRPGGameMode::AShootingRPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
