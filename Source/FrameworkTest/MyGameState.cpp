// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"
#include "Engine/Engine.h"

void AMyGameState::RegisterPickup(int32 Value)
{
	TotalPickupsCollected++;
	TotalPickupValue += Value;

	const FString Msg = FString::Printf(
		TEXT("GameState : %d pickups, valeur totale %d"),
		TotalPickupsCollected, TotalPickupValue);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Yellow, Msg);
	}
}