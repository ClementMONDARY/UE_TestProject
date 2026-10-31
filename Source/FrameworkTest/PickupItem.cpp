// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "MyCharacter.h"
#include "MyGameState.h"
#include "MyPlayerState.h"

APickupItem::APickupItem()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->InitSphereRadius(80.f);
	Sphere->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OnSphereBeginOverlap);
}

void APickupItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
	if (!Character) return; // ce n'est pas notre joueur : on ignore

	if (AMyGameState* GS = GetWorld()->GetGameState<AMyGameState>())
	{
		GS->RegisterPickup(Value);
	}
	if (AMyPlayerState* PS = Character->GetPlayerState<AMyPlayerState>())
	{
		PS->AddPickup();
	}

	Destroy();
}

