// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabbastic.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UGrabbastic : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabbastic();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	/// player viewpoint
	FVector  PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	float Reach = 200.f;
	
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent*		InputComponent = nullptr;

	// Ray-cast and grab what's in reach
	void Grab();

	// Call when grab is released
	void Release();
	
	void FindPhysicsHandleComponent();

	// Setup (assumed) attached input component
	void SetupInputComponent();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	// Populate viewpoint location and rotation variables
	void GetPlayerViewpoint();

	// Where is the player looking? Affected by range.
	FVector GetPlayerTargetLocation();

	// Returns start of current reach line
	FVector GetReachLineStart();

	// Returns end of current reach line
	FVector GetReachLineEnd();
};
