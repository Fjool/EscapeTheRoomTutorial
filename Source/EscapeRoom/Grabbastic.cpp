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
	if (PhysicsHandle)
	{
		// Physics handle is found
	}
	else
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
		UE_LOG(LogTemp, Warning, TEXT("Found input component for: %s"), *GetOwner()->GetName())

			/// Bind the input action
			InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabbastic::Grab);
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
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit		 = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if (ActorHit)
	{
		// Attach physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabbastic::Release()
{
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabbastic::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	/// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		/// Get Player view point this tick
		FVector  PlayerViewpointLocation;
		FRotator PlayerViewpointRotation;

		/// Get player viewpoint this tick
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT PlayerViewpointLocation,
			OUT PlayerViewpointRotation
		);

		///	move the object that we're holding
		PhysicsHandle->SetTargetLocation(FVector(PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach));
	}
}

const FHitResult UGrabbastic::GetFirstPhysicsBodyInReach()
{
	/// Get Player view point this tick
	FVector  PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	/// Get player viewpoint this tick
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation
	);

	/// Determine where the player's focus point ends
	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;

	/// Set up query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line trace out to reach
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewpointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/// See what we hit
	AActor *ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *ActorHit->GetName())
	}

	return Hit;
}