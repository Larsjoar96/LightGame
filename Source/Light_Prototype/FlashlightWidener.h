// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "FlashlightWidener.generated.h"

/**
 * 
 */
UCLASS()
class LIGHT_PROTOTYPE_API AFlashlightWidener : public APickup
{
	GENERATED_BODY()

public:
	AFlashlightWidener();

	// DON'T USE UFUNCTION() FOR OVERRIDES 
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
							    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
