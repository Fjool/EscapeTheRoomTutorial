// Fill out your copyright notice in the Description page of Project Settings.

#include "EscapeRoom.h"
#include "DoorOpener.h"

#define OUT

// Sets default values for this component's properties
UDoorOpener::UDoorOpener()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorOpener::BeginPlay()
{
	Super::BeginPlay();

	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Pressure Plate not assigned to %s"), *GetOwner()->GetName())
	}
}

float UDoorOpener::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	if (PressurePlate)
	{
		// find all the overlapping actors
		TArray<AActor*> OverlappingActors;

		PressurePlate->GetOverlappingActors(OUT OverlappingActors);

		// iterate through them adding their masses
		for (const auto& Actor : OverlappingActors)
		{		
			TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}
	return TotalMass;
}

// Called every frame
void UDoorOpener::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	// poll the pressure plate
	if (GetTotalMassOfActorsOnPlate() >= TriggerMass)
	{	OnOpen.Broadcast();
	}
	else
	{	OnClose.Broadcast();
	}
}

