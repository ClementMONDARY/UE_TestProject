// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Door.generated.h"

class UStaticMeshComponent;
class UTimelineComponent;
class UCurveFloat;

/**
 * Door that opens / closes when interacted with.
 * The rotation is animated by a Timeline driven by a float curve (0 = closed, 1 = open).
 */
UCLASS()
class FRAMEWORKTEST_API ADoor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ADoor();


	// --- IInteractable ---

	virtual void Interact_Implementation(AActor* InstigatorActor) override;
	virtual bool CanInteract_Implementation(AActor* InstigatorActor) const override { return !bIsLocked; }
	virtual FText GetInteractionText_Implementation() const override { return bIsOpen ? CloseText : OpenText; }

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// --- Components ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	TObjectPtr<USceneComponent> Root;

	// Rotation pivot: place it where the hinges are
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	TObjectPtr<USceneComponent> Hinge;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	TObjectPtr<UTimelineComponent> DoorTimeline;


	// --- Settings ---

	// Curve from 0 (closed) to 1 (open). Its duration and shape define the animation
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Door|Animation")
	TObjectPtr<UCurveFloat> OpenCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door", meta=(Units="deg"))
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	bool bIsLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	FText OpenText = NSLOCTEXT("Door", "OpenText", "Ouvrir");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	FText CloseText = NSLOCTEXT("Door", "CloseText", "Fermer");


	// --- State ---

	UPROPERTY(ReplicatedUsing=OnRep_IsOpen)
	bool bIsOpen = false;

	UFUNCTION()
	void OnRep_IsOpen();

private:
	void PlayDoorAnimation();

	// Called by the Timeline every frame while it plays (Alpha goes from 0 to 1)
	UFUNCTION()
	void HandleDoorProgress(float Alpha);
};
