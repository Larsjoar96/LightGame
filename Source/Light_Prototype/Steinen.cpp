// Fill out your copyright notice in the Description page of Project Settings.


#include "Steinen.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ASteinen::ASteinen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ASteinen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASteinen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

