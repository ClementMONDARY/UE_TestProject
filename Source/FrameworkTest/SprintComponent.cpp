// Fill out your copyright notice in the Description page of Project Settings.

#include "SprintComponent.h"
#include "StaminaComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

USprintComponent::USprintComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
}

void USprintComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	StaminaComponent = GetOwner()->FindComponentByClass<UStaminaComponent>();

	if (!OwnerCharacter || !StaminaComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("SprintComponent needs a Character owner with a StaminaComponent (%s)"), *GetNameSafe(GetOwner()));
		return;
	}

	DefaultWalkSpeed = OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed;
}

void USprintComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USprintComponent, bIsSprinting);
}


// --- Public API ---

void USprintComponent::StartSprint()
{
	if (bIsSprinting || !StaminaComponent || !StaminaComponent->HasEnoughStamina(MinStaminaToStart)) { return; }

	// Apply locally right away so a player feels no delay, then tell the server
	SetSprinting(true);
	if (!GetOwner()->HasAuthority()) { ServerSetSprinting(true); }
}

void USprintComponent::StopSprint()
{
	if (!bIsSprinting) { return; }

	SetSprinting(false);
	if (!GetOwner()->HasAuthority()) { ServerSetSprinting(false); }
}

void USprintComponent::ServerSetSprinting_Implementation(bool bNewSprinting)
{
	SetSprinting(bNewSprinting);
}


// --- State ---

void USprintComponent::SetSprinting(bool bNewSprinting)
{
	bIsSprinting = bNewSprinting;
	ApplyMovementSpeed();

	// Only tick (and spend stamina) on the server, and only while sprinting
	if (GetOwner()->HasAuthority())
	{
		SetComponentTickEnabled(bIsSprinting);
	}
}

void USprintComponent::OnRep_IsSprinting()
{
	ApplyMovementSpeed();
}

void USprintComponent::ApplyMovementSpeed() const
{
	if (!OwnerCharacter) { return; }
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? SprintSpeed : DefaultWalkSpeed;
}


// --- Stamina cost (server only, while sprinting) ---

void USprintComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!StaminaComponent) { return; }

	// Sprinting only costs stamina while actually moving
	const bool bIsMoving = OwnerCharacter->GetVelocity().SizeSquared2D() > 1.f;
	if (bIsMoving)
	{
		StaminaComponent->ConsumeStamina(StaminaCostPerSecond * DeltaTime);
	}

	// Out of stamina: force stop (replicates to the owning client)
	if (StaminaComponent->GetStamina() <= 0.f)
	{
		SetSprinting(false);
	}
}
