// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Components/ActorComponent.h"
#include "DoorOpener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

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

	UPROPERTY(BlueprintAssignable) FDoorEvent OnOpen;
	UPROPERTY(BlueprintAssignable) FDoorEvent OnClose;

private:
	// Set the Angle of the door
	void SetDoorAngle(float NewAngle);

	// Trigger volume which causes this door to open
	UPROPERTY(EditAnywhere)
	ATriggerVolume *PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float TriggerMass = 40.f;

	// returns total mass of actors in Kg
	float GetTotalMassOfActorsOnPlate();

};
