// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootingRPGGameMode.h"
#include "ShootingRPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShootingRPGGameMode::AShootingRPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/_Blueprints/Character/BP_MainChar"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
