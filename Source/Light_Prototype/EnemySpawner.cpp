// Fill out your copyright notice in the Description page of Project Settings.

#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "EnemySpawner.h"
#include "EngineUtils.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StartPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StartPoint"));
	SetRootComponent(StartPoint);
	StartPoint->bHiddenInGame = true;

	EndPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndPoint"));
	EndPoint->SetupAttachment(GetRootComponent());
	EndPoint->SetRelativeLocation(FVector(500.0f, 0.0f, 300.0f));
	EndPoint->bHiddenInGame = true;

	CurrentIndex = 0;
	MaxIndex = 0;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* CurrentLevel = GetWorld();

	// Fill up 'Enemy' array with all instances of type 'AEnemy' class.
	// Could also use 'GetAllActorsOfClass' function. This for loop is basically
	// what is inside that optional function, but I want more control of each element.
	// StaticClass() is a member function in 'UObject' which returns an object of type AEnemy.
	for (TActorIterator<AEnemy> Enemy(CurrentLevel, AEnemy::StaticClass()); Enemy; ++Enemy)
	{
		AEnemy* EnemyPtr = *Enemy;
		if (EnemyPtr != NULL)
		{
			Enemies.Add(EnemyPtr);
		}
	}

	MaxIndex = Enemies.Num();
	SpawnEnemies();
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SpawnEnemies()
{
	float RandomSpawnDelay = 0.4f;

	UWorld* CurrentLevel = GetWorld();
	UE_LOG(LogTemp, Warning, TEXT("Max index: %i"), MaxIndex)

	if (CurrentIndex < MaxIndex)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning index: %i"), CurrentIndex)

		// Spawning enemies in order (starting at index 0) by changing their transform
		// (enemies are already instantiated at start of level, this is to increase performance)
		AEnemy* SpawnedEnemy = Enemies[CurrentIndex];
		SpawnedEnemy->SetActorLocation(StartPoint->GetComponentLocation());

		// Add force and shit

		CurrentIndex++;
		RandomSpawnDelay *= UKismetMathLibrary::RandomFloat();

		// Add a little delay between each spawn with minimum delay being 0.15 seconds and max being 0.55 seconds
		CurrentLevel->GetTimerManager().SetTimer(MyTimerHandle, this, &AEnemySpawner::SpawnEnemies, (2.15f + RandomSpawnDelay));
	}

}