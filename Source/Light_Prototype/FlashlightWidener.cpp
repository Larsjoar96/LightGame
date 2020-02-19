// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashlightWidener.h"

AFlashlightWidener::AFlashlightWidener()
{

}


void AFlashlightWidener::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
									    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Warning, TEXT("Flashlight Widener: OnOverlapBegin called"))

	// Add functionalities for Flashlight Widener pickup

	// Remove pickup instance from scene. This will also remove parent class.
	Destroy();
}