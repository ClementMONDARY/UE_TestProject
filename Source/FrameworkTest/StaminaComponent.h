// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChangedSignature, float, NewValue, float, MaxValue);

/**
 * Generic stamina pool (player or AI).
 * Any action can consume it; consuming always restarts the regen delay.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FRAMEWORKTEST_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStaminaComponent();

	UPROPERTY(BlueprintAssignable, Category="Stamina")
	FOnStaminaChangedSignature OnStaminaChanged;

	UFUNCTION(BlueprintPure, Category="Stamina")
	float GetStamina() const { return Stamina; }

	UFUNCTION(BlueprintPure, Category="Stamina")
	float GetMaxStamina() const { return MaxStamina; }

	UFUNCTION(BlueprintPure, Category="Stamina")
	float GetStaminaPercent() const { return MaxStamina > 0.f ? Stamina / MaxStamina : 0.f; }

	UFUNCTION(BlueprintPure, Category="Stamina")
	bool HasEnoughStamina(float Amount) const { return Stamina >= Amount; }

	UFUNCTION(BlueprintCallable, Category="Stamina")
	void ConsumeStamina(float Amount);

	UFUNCTION(BlueprintCallable, Category="Stamina")
	void RestoreStamina(float Amount);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// --- Settings ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stamina", meta=(ClampMin="1"))
	float MaxStamina = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stamina|Regen", meta=(ClampMin="0", Units="s"))
	float RegenDelay = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stamina|Regen", meta=(ClampMin="0"))
	float RegenPerSecond = 15.f;


	// --- Runtime state ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Stamina, Category="Stamina")
	float Stamina = 0.f;

	UFUNCTION()
	void OnRep_Stamina();

private:
	bool HasAuthority() const { return GetOwner() && GetOwner()->HasAuthority(); }
	void SetStamina(float NewValue);

	float RegenDelayRemaining = 0.f;
};
