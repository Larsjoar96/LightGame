// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaManager.h"
#include "Components/BoxComponent.h"

// Sets default values
AArenaManager::AArenaManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the component 
	ArenaVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ArenaVolume"));
	ArenaVolume->SetBoxExtent(FVector(500.0f, 700.0f, 200.0f));
	ArenaVolume->bHiddenInGame = false;
	SetRootComponent(ArenaVolume);

	EnemiesLeft = 0;
}

// Called when the game starts or when spawned
void AArenaManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArenaManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Will be called by enemies
void AArenaManager::IncrementEnemies()
{
	EnemiesLeft++;
	UE_LOG(LogTemp, Warning, TEXT("Enemies: %i"), EnemiesLeft)
}

// Will be called by enemies
void AArenaManager::DecrementEnemies()
{
	EnemiesLeft--;
	UE_LOG(LogTemp, Warning, TEXT("Enemies: %i"), EnemiesLeft)

	if (EnemiesLeft <= 0)
	{
		// SPAWN NEXT WAVE HERE FROM ENEMYSPAWNER
	}

}

