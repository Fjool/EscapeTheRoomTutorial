// Fill out your copyright notice in the Description page of Project Settings.

#include "EscapeRoom.h"
#include "Grabbastic.h"

#define OUT

// Sets default values for this component's properties
UGrabbastic::UGrabbastic()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabbastic::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

/// Look for the required physics handler
void UGrabbastic::FindPhysicsHandleComponent()
{
	/// Look for attached Physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics handle for not found for: %s"), *GetOwner()->GetName())
	}
}

/// Look for the required input component
void UGrabbastic::SetupInputComponent()
{
	/// Look for attached input component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed,  this, &UGrabbastic::Grab   );
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabbastic::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component not found for: %s"), *GetOwner()->GetName())
	}
}

void UGrabbastic::Grab()
{
	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult		 = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case
	auto ActorHit		 = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, //  no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabbastic::Release()
{
	PhysicsHandle->ReleaseComponent();
}

void UGrabbastic::GetPlayerViewpoint()
{
	/// Get player viewpoint this tick
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation
	);
}

FVector UGrabbastic::GetReachLineStart()
{
	GetPlayerViewpoint();
	return PlayerViewpointLocation;
}

FVector UGrabbastic::GetReachLineEnd()
{
	GetPlayerViewpoint();
	return PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;
}

// Called every frame
void UGrabbastic::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	/// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		///	move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

const FHitResult UGrabbastic::GetFirstPhysicsBodyInReach()
{
	/// Set up query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line trace out to reach
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	return HitResult;
}
