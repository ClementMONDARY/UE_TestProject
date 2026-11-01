// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInputBinder.generated.h"

class UEnhancedInputComponent;

UINTERFACE(MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UPlayerInputBinder : public UInterface
{
	GENERATED_BODY()
};

/**
 * Implemented by components that bind their own player input.
 * AMyCharacter calls SetupPlayerInput on every component implementing it.
 */
class FRAMEWORKTEST_API IPlayerInputBinder
{
	GENERATED_BODY()

public:
	virtual void SetupPlayerInput(UEnhancedInputComponent* Input) = 0;
};
