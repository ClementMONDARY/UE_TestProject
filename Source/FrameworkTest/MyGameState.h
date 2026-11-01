// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class FRAMEWORKTEST_API AMyGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Pickups")
	void RegisterPickup(int32 Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickups")
	int32 TotalPickupsCollected = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickups")
	int32 TotalPickupValue = 0;
};