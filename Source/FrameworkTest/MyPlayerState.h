// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChangedSignature, int32, NewValue, int32, MaxValue);

/**
 *
 */
UCLASS()
class FRAMEWORKTEST_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Pickups")
	void AddPickup() { ++PlayerPickupCount; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickups")
	int32 PlayerPickupCount = 0;


	// --- Events ---

	UPROPERTY(BlueprintAssignable, Category="Stats|Events")
	FOnStatChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Stats|Events")
	FOnStatChangedSignature OnShieldChanged;


	// --- Health ---

	UFUNCTION(BlueprintPure, Category="Stats|Health")
	int32 GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure, Category="Stats|Health")
	int32 GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category="Stats|Health")
	float GetHealthPercent() const { return MaxHealth > 0 ? static_cast<float>(Health) / MaxHealth : 0.f; }

	UFUNCTION(BlueprintPure, Category="Stats|Health")
	bool IsDead() const { return Health <= 0; }

	UFUNCTION(BlueprintCallable, Category="Stats|Health")
	void ApplyDamage(int32 Amount);

	UFUNCTION(BlueprintCallable, Category="Stats|Health")
	void Heal(int32 Amount);


	// --- Shield ---

	UFUNCTION(BlueprintPure, Category="Stats|Shield")
	int32 GetShield() const { return Shield; }

	UFUNCTION(BlueprintPure, Category="Stats|Shield")
	int32 GetMaxShield() const { return MaxShield; }

	UFUNCTION(BlueprintPure, Category="Stats|Shield")
	float GetShieldPercent() const { return MaxShield > 0 ? static_cast<float>(Shield) / MaxShield : 0.f; }

	UFUNCTION(BlueprintCallable, Category="Stats|Shield")
	void RestoreShield(int32 Amount);


	// --- Attack ---

	UFUNCTION(BlueprintPure, Category="Stats|Attack")
	int32 GetAttack() const { return Attack; }

	UFUNCTION(BlueprintCallable, Category="Stats|Attack")
	void ModifyAttack(int32 Delta);

protected:
	virtual void PostInitializeComponents() override;

	// Max values: editable in the Blueprint defaults, read-only in game
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats", meta=(ClampMin="1"))
	int32 MaxHealth = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats", meta=(ClampMin="0"))
	int32 MaxShield = 100;

	// Current values: visible but never editable, set from the max values in BeginPlay
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Stats")
	int32 Health = MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Shield, Category="Stats")
	int32 Shield = MaxShield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Stats", meta=(ClampMin="0"))
	int32 Attack = 15;

	// Called on clients when the replicated value arrives
	UFUNCTION()
	void OnRep_Health();

	UFUNCTION()
	void OnRep_Shield();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	// Single entry point to change a stat on the server: clamp + broadcast
	void SetHealth(int32 NewValue);
	void SetShield(int32 NewValue);
};
