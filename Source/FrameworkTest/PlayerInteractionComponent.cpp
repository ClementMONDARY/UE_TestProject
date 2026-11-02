// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInteractionComponent.h"
#include "Interactable.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"

UPlayerInteractionComponent::UPlayerInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Required for the Server RPC to work on a component
	SetIsReplicatedByDefault(true);
}

void UPlayerInteractionComponent::SetupPlayerInput(UEnhancedInputComponent* Input)
{
	if (!InteractAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: InteractAction is not set (%s)"), *GetNameSafe(GetOwner()));
		return;
	}

	Input->BindAction(InteractAction, ETriggerEvent::Started, this, &UPlayerInteractionComponent::OnInteractPressed);
}

void UPlayerInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateFocus();
}


// --- Focus (local player only) ---

void UPlayerInteractionComponent::UpdateFocus()
{
	// Only the player controlling this pawn on this machine needs to know what he aims at
	const APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn || !Pawn->IsLocallyControlled() || !Pawn->GetController()) { return; }

	FVector ViewLocation;
	FRotator ViewRotation;
	Pawn->GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
	const FVector TraceEnd = ViewLocation + ViewRotation.Vector() * InteractionDistance;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractionTrace), false, GetOwner());
	FHitResult Hit;

	AActor* NewFocus = nullptr;
	if (GetWorld()->LineTraceSingleByChannel(Hit, ViewLocation, TraceEnd, TraceChannel, Params))
	{
		if (IsInteractable(Hit.GetActor()))
		{
			NewFocus = Hit.GetActor();
		}
	}
	FocusedActor = NewFocus;

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), ViewLocation, TraceEnd, FocusedActor ? FColor::Green : FColor::Red, false, 0.f, 0, 1.f);
	}
}


// --- Interaction ---

void UPlayerInteractionComponent::OnInteractPressed()
{
	if (!FocusedActor) { return; }
	ServerInteract(FocusedActor);
}

void UPlayerInteractionComponent::ServerInteract_Implementation(AActor* Target)
{
	// Never trust the client: check everything again on the server
	if (!IsInteractable(Target) || !IsInRange(Target)) { return; }

	IInteractable::Execute_Interact(Target, GetOwner());
}

bool UPlayerInteractionComponent::IsInteractable(AActor* Target) const
{
	// Implements<>() works for interfaces implemented in C++ AND in Blueprint
	return IsValid(Target)
		&& Target->Implements<UInteractable>()
		&& IInteractable::Execute_CanInteract(Target, GetOwner());
}

bool UPlayerInteractionComponent::IsInRange(const AActor* Target) const
{
	// The trace starts from the eyes and hits the edge of the target: keep some margin
	const float MaxDistance = InteractionDistance * 1.5f;
	return FVector::Dist(GetOwner()->GetActorLocation(), Target->GetActorLocation()) <= MaxDistance;
}
