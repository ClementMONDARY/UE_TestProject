// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Net/UnrealNetwork.h"

ADoor::ADoor()
{
	bReplicates = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Hinge = CreateDefaultSubobject<USceneComponent>(TEXT("Hinge"));
	Hinge->SetupAttachment(Root);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(Hinge);

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		DoorTimeline->SetIsReplicated(false);
	}

	if (!OpenCurve)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door: OpenCurve is not set, the door will snap open/closed (%s)"), *GetName());
		return;
	}

	// Tell the timeline: "while playing, read OpenCurve and send its value to HandleDoorProgress"
	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, FName("HandleDoorProgress"));
	DoorTimeline->AddInterpFloat(OpenCurve, ProgressFunction);

	// The timeline lasts as long as the curve (its last key)
	DoorTimeline->SetTimelineLengthMode(TL_LastKeyFrame);
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADoor, bIsOpen);
}


// --- Interaction (server) ---

void ADoor::Interact_Implementation(AActor* InstigatorActor)
{
	bIsOpen = !bIsOpen;

	// OnRep is not called on the server: play the animation here too
	PlayDoorAnimation();
}

void ADoor::OnRep_IsOpen()
{
	PlayDoorAnimation();
}


// --- Animation ---

void ADoor::PlayDoorAnimation()
{
	if (!OpenCurve)
	{
		HandleDoorProgress(bIsOpen ? 1.f : 0.f);
		return;
	}

	// Play / Reverse continue from the current position:
	// closing a half-open door starts from where it is, no jump
	if (bIsOpen)
	{
		DoorTimeline->Play();
	}
	else
	{
		DoorTimeline->Reverse();
	}
}

void ADoor::HandleDoorProgress(float Alpha)
{
	Hinge->SetRelativeRotation(FRotator(0.f, OpenAngle * Alpha, 0.f));
}
