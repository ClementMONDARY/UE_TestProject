// Fill out your copyright notice in the Description page of Project Settings.

#include "StaminaComponent.h"
#include "Net/UnrealNetwork.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UStaminaComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Runs before any BeginPlay, after the Blueprint defaults are applied
	Stamina = MaxStamina;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// Regen only runs on the server
	SetComponentTickEnabled(HasAuthority());
}

void UStaminaComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UStaminaComponent, Stamina);
}

void UStaminaComponent::ConsumeStamina(float Amount)
{
	if (!HasAuthority() || Amount <= 0.f) { return; }

	SetStamina(Stamina - Amount);
	RegenDelayRemaining = RegenDelay;
}

void UStaminaComponent::RestoreStamina(float Amount)
{
	if (!HasAuthority() || Amount <= 0.f) { return; }
	SetStamina(Stamina + Amount);
}

void UStaminaComponent::SetStamina(float NewValue)
{
	const float Clamped = FMath::Clamp(NewValue, 0.f, MaxStamina);
	if (FMath::IsNearlyEqual(Clamped, Stamina)) { return; }

	Stamina = Clamped;
	OnStaminaChanged.Broadcast(Stamina, MaxStamina);
}

void UStaminaComponent::OnRep_Stamina()
{
	OnStaminaChanged.Broadcast(Stamina, MaxStamina);
}

void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Wait before regenerating
	if (RegenDelayRemaining > 0.f)
	{
		RegenDelayRemaining -= DeltaTime;
		return;
	}

	if (Stamina < MaxStamina)
	{
		SetStamina(Stamina + RegenPerSecond * DeltaTime);
	}
}
