// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "PickupItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class FRAMEWORKTEST_API APickupItem : public AActor, public IInteractable
{
	GENERATED_BODY()
public:
	APickupItem();


	// --- IInteractable ---

	virtual void Interact_Implementation(AActor* InstigatorActor) override;
	virtual bool CanInteract_Implementation(AActor* InstigatorActor) const override { return true; }
	virtual FText GetInteractionText_Implementation() const override { return InteractionText; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickup")
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickup")
	TObjectPtr<UStaticMeshComponent> Mesh;

	// Modifiable pour CHAQUE instance posée dans le niveau
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	int32 Value = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	FText InteractionText = NSLOCTEXT("Pickup", "InteractionText", "Ramasser");
};
