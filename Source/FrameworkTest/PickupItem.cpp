// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "MyGameState.h"
#include "MyPlayerState.h"

APickupItem::APickupItem()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->InitSphereRadius(80.f);

	// The sphere is only used to be hit by the interaction trace
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void APickupItem::Interact_Implementation(AActor* InstigatorActor)
{
	const APawn* Pawn = Cast<APawn>(InstigatorActor);
	if (!Pawn) { return; }

	if (AMyGameState* GS = GetWorld()->GetGameState<AMyGameState>())
	{
		GS->RegisterPickup(Value);
	}
	if (AMyPlayerState* PS = Pawn->GetPlayerState<AMyPlayerState>())
	{
		PS->AddPickup();
	}

	Destroy();
}
