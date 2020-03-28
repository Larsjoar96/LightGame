// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "Pickup_LaserWidener.h"

APickup_LaserWidener::APickup_LaserWidener()
{

}

void APickup_LaserWidener::BeginPlay()
{
	Super::BeginPlay();

	// Start checking for OverlapBegin events
	Collider->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapBegin);
}


void APickup_LaserWidener::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	Player = Cast <AMyPlayer>(OtherActor);
	Player->PickupEventBiggerLaser();
	this->Destroy();

	UE_LOG(LogTemp, Warning, TEXT("LaserWidener: OnOverlapBegin called"))

		// Functionalities for 'FlashlightWidener' is implemented in the BluePrint
}