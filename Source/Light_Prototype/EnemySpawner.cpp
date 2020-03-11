// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
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

	// Default 'none' value, as a safety if you forgot to assign it in the editor
	SpawnLabel = ESpawnLabel::ESL_None;
		
	CurrentIndex = 0;
	MaxIndex = 0;
	CurrentWave = 0;

	Seconds = 0; // ONLY DEBUGGING!

	// Assignes all elements of 'Waves' with the value 0.
	// (IMPORTANT: for-loop does not update dynamically to the size of array... will be fixed)
	for (int32 i{ 0 }; i < 3; i++)
	{
		Waves[i] = 0;
	}
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	// 'SpawnInt' and 'EnemyInt' are the enum constants of 'ESpawnLabel' and 'EEnemyLabel' converted
	// over to int32. The reason behind this conversion is explained further down in the if-statement.
	UWorld* CurrentLevel = GetWorld();
	int32 SpawnInt = (int32)SpawnLabel;
	int32 EnemyInt{};

	// Fill up 'Enemy' array with all instances of type 'AEnemy' class.
	// Could also use 'GetAllActorsOfClass' function. This for loop is basically
	// what is inside that optional function, but I want more control of each element.
	// StaticClass() is a member function in 'UObject' which returns an object of type AEnemy.
	for (TActorIterator<AEnemy> Enemy(CurrentLevel, AEnemy::StaticClass()); Enemy; ++Enemy)
	{
		AEnemy* EnemyPtr = *Enemy;
		if (EnemyPtr != NULL)
		{
			// Converting 'EnemyLabel' to int32 and storing it in 'EnemyInt'
			EnemyInt = (int32)EnemyPtr->EnemyLabel;

			// Checks if the enemy is supposed to be spawned at this spawner.
			// This is the reason why I converted the enum constants over to int32.
			// Both sides of the '==' have to be the same data type.
			if (SpawnInt == EnemyInt) Enemies.Add(EnemyPtr);
		}
	}

	// Start spawning enemies 
	SpawnNextWave();
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// ONLY DEBUGGING!
	Seconds += DeltaTime;

	if (Seconds >= 20.0f)
	{
		Seconds = 0.0f;
		SpawnNextWave();
	}
	// ONLY DEBUGGING!

}

void AEnemySpawner::SpawnNextWave()
{
	// Spawn Delay
	float RandomSpawnDelay = 0.5f;
	RandomSpawnDelay *= UKismetMathLibrary::RandomFloat();

	// Resets values for next wave
	CurrentIndex = 0;
	MaxIndex = Waves[CurrentWave];

	// This function will be called when player enters room
	GetWorld()->GetTimerManager().SetTimer(MyTimerHandle, this, &AEnemySpawner::SpawnEnemies, (2.0f + RandomSpawnDelay));
	CurrentWave++; 
}

// Recursive function
void AEnemySpawner::SpawnEnemies()
{
	float RandomSpawnDelay = 0.5f;
	float RandomLaunchForce{};

	UWorld* CurrentLevel = GetWorld();

	// REMEMBER TO ALSO MAKE LOGIC FOR: when there are no more waves
	// If there are supposed to spawn enemies this wave
	if (MaxIndex > 0)
	{
		if (CurrentIndex < MaxIndex)
		{
			// Spawning enemies in order (starting at index 0) by changing their transform
			// (enemies are already instantiated at start of level, this is to increase performance)
			AEnemy* SpawnedEnemy = Enemies[CurrentIndex];
			if (SpawnedEnemy)
			{
				UE_LOG(LogTemp, Warning, TEXT("Spawning index: %i"), CurrentIndex)

				// Teleport enemy to spawn location and set its rotation to be equal to spawn rotation
				SpawnedEnemy->SetActorLocation(StartPoint->GetComponentLocation());
				SpawnedEnemy->SetActorRotation(this->GetActorRotation());

				// Assign random values for both variables
				RandomSpawnDelay *= UKismetMathLibrary::RandomFloat();
				RandomLaunchForce = UKismetMathLibrary::RandomFloatInRange(0.9f, 1.1f);

				// 'LaunchVector' is calculated (by trial and error) to launch in the correct direction with the correct force
				UCharacterMovementComponent* EnemyMovementComponent;
				FVector LaunchDirection = this->GetActorForwardVector();
				FVector LaunchVector = FVector(34000.0f * LaunchDirection.X, 34000.0f * LaunchDirection.Y, 100000.0f) * RandomLaunchForce;

				// Cast from PawnMovementComponent to CharacterMovementComponent to get access to 'AddImpulse()'
				// I use 'AddImpulse()' to launch the enemy towards the platform at spawn time
				EnemyMovementComponent = Cast<UCharacterMovementComponent>(SpawnedEnemy->GetMovementComponent());
				EnemyMovementComponent->AddImpulse(LaunchVector, false);

				CurrentIndex++;
				// Add a little delay between each spawn with minimum delay being 0.35 seconds and max being 0.85 seconds
				CurrentLevel->GetTimerManager().SetTimer(MyTimerHandle, this, &AEnemySpawner::SpawnEnemies, (0.35f + RandomSpawnDelay));
			}
		}
	}

}