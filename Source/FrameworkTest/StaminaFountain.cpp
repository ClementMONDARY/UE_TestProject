// Fill out your copyright notice in the Description page of Project Settings.

#include "StaminaFountain.h"
#include "StaminaComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

AStaminaFountain::AStaminaFountain()
{
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void AStaminaFountain::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AStaminaFountain, bIsAvailable);
}

bool AStaminaFountain::CanInteract_Implementation(AActor* InstigatorActor) const
{
	if (!bIsAvailable || !InstigatorActor) { return false; }

	// Only actors with stamina, and only if it is not already full
	const UStaminaComponent* Stamina = InstigatorActor->FindComponentByClass<UStaminaComponent>();
	return Stamina && Stamina->GetStamina() < Stamina->GetMaxStamina();
}

void AStaminaFountain::Interact_Implementation(AActor* InstigatorActor)
{
	UStaminaComponent* Stamina = InstigatorActor->FindComponentByClass<UStaminaComponent>();
	if (!Stamina) { return; }

	Stamina->RestoreStamina(StaminaAmount);

	// Cooldown: unavailable for a few seconds (a timer of 0 s would never fire)
	if (Cooldown <= 0.f) { return; }

	bIsAvailable = false;
	GetWorldTimerManager().SetTimer(CooldownTimer, this, &AStaminaFountain::EndCooldown, Cooldown, false);
}

void AStaminaFountain::EndCooldown()
{
	bIsAvailable = true;
}
