// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInputBinder.h"
#include "PlayerInteractionComponent.generated.h"

class UInputAction;
class APlayerController;
class UInteractionPromptWidget;

/**
 * Player side of the interaction system:
 * traces from the player's view every frame to find what is aimed at,
 * shows the "[E] ..." prompt, and asks the server to interact when the input is pressed.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FRAMEWORKTEST_API UPlayerInteractionComponent : public UActorComponent, public IPlayerInputBinder
{
	GENERATED_BODY()

public:
	UPlayerInteractionComponent();

	virtual void SetupPlayerInput(UEnhancedInputComponent* Input) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Interactable actor currently aimed at (null if none)
	UFUNCTION(BlueprintPure, Category="Interaction")
	AActor* GetFocusedActor() const { return FocusedActor; }

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	// --- Settings ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction", meta=(ClampMin="0", Units="cm"))
	float InteractionDistance = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	// Widget Blueprint (child of InteractionPromptWidget) used for the "[E] ..." prompt
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<UInteractionPromptWidget> PromptWidgetClass;

	// Draws the trace: green when aiming at an interactable, red otherwise
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction|Debug")
	bool bDrawDebug = false;


	// --- Network ---

	// Called by the owning client; when called on the server it simply runs locally
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* Target);

private:
	void UpdateFocus();
	void UpdatePrompt(APlayerController* PlayerController);
	void OnInteractPressed();

	bool IsInteractable(AActor* Target) const;
	bool IsInRange(const AActor* Target) const;

	// Display name of the key bound to InteractAction ("E")
	FText GetInteractKeyName(const APlayerController* PlayerController) const;

	UPROPERTY()
	TObjectPtr<AActor> FocusedActor;

	UPROPERTY()
	TObjectPtr<UInteractionPromptWidget> PromptWidget;

	FText InteractKeyName;
};
