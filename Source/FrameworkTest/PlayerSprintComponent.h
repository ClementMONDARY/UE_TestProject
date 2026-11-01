// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SprintComponent.h"
#include "PlayerInputBinder.h"
#include "PlayerSprintComponent.generated.h"

class UInputAction;

/**
 * Player version of the sprint: binds its own input action, no Event Graph needed.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FRAMEWORKTEST_API UPlayerSprintComponent : public USprintComponent, public IPlayerInputBinder
{
	GENERATED_BODY()

public:
	virtual void SetupPlayerInput(UEnhancedInputComponent* Input) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> SprintAction;
};
