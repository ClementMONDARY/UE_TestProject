// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerState.h"
#include "Net/UnrealNetwork.h"

void AMyPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		SetHealth(MaxHealth);
		SetShield(MaxShield);
	}
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyPlayerState, Health);
	DOREPLIFETIME(AMyPlayerState, Shield);
	DOREPLIFETIME(AMyPlayerState, Attack);
}


// --- Health ---

void AMyPlayerState::ApplyDamage(int32 Amount)
{
	if (!HasAuthority() || Amount <= 0) { return; }

	// Shield first
	const int32 DamageAbsorbedByShield = FMath::Min(Shield, Amount);
	SetShield(Shield - DamageAbsorbedByShield);

	// Then Health after
	const int32 RemainingDamage = Amount - DamageAbsorbedByShield;
	SetHealth(Health - RemainingDamage);
}

void AMyPlayerState::Heal(int32 Amount)
{
	if (!HasAuthority() || Amount <= 0) { return; }
	SetHealth(Health + Amount);
}

void AMyPlayerState::SetHealth(int32 NewValue)
{
	const int32 Clamped = FMath::Clamp(NewValue, 0, MaxHealth);
	if (Clamped == Health) { return; }

	Health = Clamped;
	OnHealthChanged.Broadcast(Health, MaxHealth);
}

void AMyPlayerState::OnRep_Health()
{
	OnHealthChanged.Broadcast(Health, MaxHealth);
}


// --- Shield ---

void AMyPlayerState::RestoreShield(int32 Amount)
{
	if (!HasAuthority() || Amount <= 0) { return; }
	SetShield(Shield + Amount);
}

void AMyPlayerState::SetShield(int32 NewValue)
{
	const int32 Clamped = FMath::Clamp(NewValue, 0, MaxShield);
	if (Clamped == Shield) { return; }

	Shield = Clamped;
	OnShieldChanged.Broadcast(Shield, MaxShield);
}

void AMyPlayerState::OnRep_Shield()
{
	OnShieldChanged.Broadcast(Shield, MaxShield);
}


// --- Attack ---

void AMyPlayerState::ModifyAttack(int32 Delta)
{
	if (!HasAuthority()) { return; }
	Attack = FMath::Max(Attack + Delta, 0);
}
