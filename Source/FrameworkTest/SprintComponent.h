// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SprintComponent.generated.h"

class ACharacter;
class UStaminaComponent;

/**
 * Generic sprint (player or AI). Requires a StaminaComponent on the same Character.
 * AI: call StartSprint / StopSprint on the server. Player: see PlayerSprintComponent.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FRAMEWORKTEST_API USprintComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USprintComponent();

	UFUNCTION(BlueprintCallable, Category="Sprint")
	void StartSprint();

	UFUNCTION(BlueprintCallable, Category="Sprint")
	void StopSprint();

	UFUNCTION(BlueprintPure, Category="Sprint")
	bool IsSprinting() const { return bIsSprinting; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// --- Settings ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sprint", meta=(ClampMin="0", Units="cm/s"))
	float SprintSpeed = 900.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sprint", meta=(ClampMin="0"))
	float StaminaCostPerSecond = 20.f;

	// Avoids start/stop flickering when stamina is almost empty
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sprint", meta=(ClampMin="0"))
	float MinStaminaToStart = 10.f;


	// --- Runtime state ---

	UPROPERTY(ReplicatedUsing=OnRep_IsSprinting)
	bool bIsSprinting = false;

	UFUNCTION()
	void OnRep_IsSprinting();

	UFUNCTION(Server, Reliable)
	void ServerSetSprinting(bool bNewSprinting);

private:
	void SetSprinting(bool bNewSprinting);
	void ApplyMovementSpeed() const;

	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<UStaminaComponent> StaminaComponent;

	float DefaultWalkSpeed = 0.f;
};
