// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "PlayerInputBinder.h"

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput)
	{
		UE_LOG(LogTemp, Warning, TEXT("MyCharacter: input component is not an EnhancedInputComponent"));
		return;
	}

	// Let every component that needs player input bind its own actions
	for (UActorComponent* Component : GetComponents())
	{
		if (IPlayerInputBinder* Binder = Cast<IPlayerInputBinder>(Component))
		{
			Binder->SetupPlayerInput(EnhancedInput);
		}
	}
}
