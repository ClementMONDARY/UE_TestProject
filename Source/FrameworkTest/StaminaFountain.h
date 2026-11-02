// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "StaminaFountain.generated.h"

class UStaticMeshComponent;

/**
 * Restores stamina of whoever interacts with it, then goes on cooldown.
 * Works with any actor that has a StaminaComponent (player or AI).
 */
UCLASS()
class FRAMEWORKTEST_API AStaminaFountain : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AStaminaFountain();


	// --- IInteractable ---

	virtual void Interact_Implementation(AActor* InstigatorActor) override;
	virtual bool CanInteract_Implementation(AActor* InstigatorActor) const override;
	virtual FText GetInteractionText_Implementation() const override { return InteractionText; }

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Fountain")
	TObjectPtr<UStaticMeshComponent> Mesh;


	// --- Settings ---

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Fountain", meta=(ClampMin="0"))
	float StaminaAmount = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Fountain", meta=(ClampMin="0", Units="s"))
	float Cooldown = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	FText InteractionText = NSLOCTEXT("Fountain", "InteractionText", "Boire");


	// --- State ---

	UPROPERTY(Replicated)
	bool bIsAvailable = true;

private:
	void EndCooldown();

	FTimerHandle CooldownTimer;
};
