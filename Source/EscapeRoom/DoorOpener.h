// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Components/ActorComponent.h"
#include "DoorOpener.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UDoorOpener : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorOpener();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void  OpenDoor();
	void CloseDoor();

protected:

	// Set the Angle of the door
	void SetDoorAngle(float NewAngle);

	// Angle to which the door opens
	UPROPERTY(EditAnywhere)
	float Angle = 90.f;

	// Trigger volume which causes this door to open
	UPROPERTY(EditAnywhere)
	ATriggerVolume *PressurePlate;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.f;

	float LastDoorOpenTime;

	// Actor that causes this plate to trigger
	AActor *ActorThatOpens;
};
