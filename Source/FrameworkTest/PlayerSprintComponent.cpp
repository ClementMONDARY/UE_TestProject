// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSprintComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

void UPlayerSprintComponent::SetupPlayerInput(UEnhancedInputComponent* Input)
{
	if (!SprintAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerSprintComponent: SprintAction is not set (%s)"), *GetNameSafe(GetOwner()));
		return;
	}

	Input->BindAction(SprintAction, ETriggerEvent::Started, this, &USprintComponent::StartSprint);
	Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &USprintComponent::StopSprint);
}
