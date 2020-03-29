// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashlightWidener.h"
#include "MyPlayer.h"

AFlashlightWidener::AFlashlightWidener()
{

}

void AFlashlightWidener::BeginPlay()
{
	Super::BeginPlay();

	// Start checking for OverlapBegin events
	Collider->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapBegin);
}


void AFlashlightWidener::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
									    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	Player = Cast <AMyPlayer>(OtherActor);
	Player->PickupEventBiggerFlashlight();
	this->Destroy();

	UE_LOG(LogTemp, Warning, TEXT("Flashlight Widener: OnOverlapBegin called"))

	// Functionalities for 'FlashlightWidener' is implemented in the BluePrint
}