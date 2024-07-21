// Copyright Epic Games, Inc. All Rights Reserved.

#include "MS_GameProjectGameMode.h"
#include "MS_GameProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMS_GameProjectGameMode::AMS_GameProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/Characters/Player/BP_Player.BP_Player'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
