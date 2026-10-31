// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "MyGameState.h"
#include "MyPlayerState.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"

AMyGameMode::AMyGameMode()
{
	GameStateClass        = AMyGameState::StaticClass();
	PlayerStateClass      = AMyPlayerState::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	DefaultPawnClass      = AMyCharacter::StaticClass();
}