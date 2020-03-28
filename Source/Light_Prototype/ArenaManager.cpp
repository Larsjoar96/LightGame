// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaManager.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnemySpawner.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AArenaManager::AArenaManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArenaVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ArenaVolume"));
	ArenaVolume->SetBoxExtent(FVector(500.0f, 700.0f, 200.0f));
	SetRootComponent(ArenaVolume);

	// Make the collision only trigger OverlapEvents whenever the player collides
	EnterArenaCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("EnterArenaCollider"));
	EnterArenaCollider->SetupAttachment(ArenaVolume);
	EnterArenaCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EnterArenaCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	EnterArenaCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	EnterArenaCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// Set position and mesh in blueprint (because it's different for each platform). Level designer's work
	EnterPlatform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnterPlatform"));
	EnterPlatform->SetupAttachment(ArenaVolume);

	// Set position and mesh in blueprint (because it's different for each platform). Level designer's work
	ExitPlatform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExitPlatform"));
	ExitPlatform->SetupAttachment(ArenaVolume);

	// Adjust size and mesh in blueprint.
	MainPlatform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainPlatform"));
	MainPlatform->SetupAttachment(ArenaVolume);

	// Assign default values
	bLowerPlatforms = false;
	bRaisePlatforms = false;
	bEncounterComplete = 0;
	AmountOfSpawners = 0;
	TimePassed = 0.0f;
	EnemiesLeft = 0;
	LoweringZ = 0.0f;
	RaisingZ = 0.0f;
}

// Called when the game starts or when spawned
void AArenaManager::BeginPlay()
{
	Super::BeginPlay();
	
	for (int32 i{ 0 }; i < AmountOfSpawners; i++)
	{
		// This will most likely cause a crash though
		if (EnemySpawners[i] == NULL) UE_LOG(LogTemp, Error, TEXT("ArenaManager: can't find reference to spawner ..."));
	}

	// AmountOfSpawners will be equal to the number of spawners assigned to 'EnemySpawners' TArray
	AmountOfSpawners = EnemySpawners.Num();

	// Enabling detection
	EnterArenaCollider->OnComponentBeginOverlap.AddDynamic(this, &AArenaManager::OnOverlapBegin);
}

// Called every frame
void AArenaManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// QUESTION: is it expensive to check a bool every frame? like I do here?

	if (bLowerPlatforms == true)
	{
		// Lower platforms
		TimePassed += DeltaTime;
		LoweringZ = (-2.6f) * UKismetMathLibrary::Cos(TimePassed - (UKismetMathLibrary::GetPI() / 2));
		EnterPlatform->SetWorldLocation(EnterPlatform->GetComponentLocation() + FVector(0.0f, 0.0f, LoweringZ));
		ExitPlatform->SetWorldLocation(ExitPlatform->GetComponentLocation() + FVector(0.0f, 0.0f, LoweringZ));
		if (TimePassed >= (UKismetMathLibrary::GetPI() / 2)) bLowerPlatforms = false;
	}

	if (bRaisePlatforms == true)
	{
		// Raise platforms
		TimePassed += DeltaTime;
		RaisingZ = (-2.6f) * UKismetMathLibrary::Cos(TimePassed + (UKismetMathLibrary::GetPI()));
		EnterPlatform->SetWorldLocation(EnterPlatform->GetComponentLocation() + FVector(0.0f, 0.0f, RaisingZ));
		ExitPlatform->SetWorldLocation(ExitPlatform->GetComponentLocation() + FVector(0.0f, 0.0f, RaisingZ));
		if (TimePassed >= (UKismetMathLibrary::GetPI() / 2)) bRaisePlatforms = false;
	}
}

// Will be called by enemies
void AArenaManager::IncrementEnemies()
{
	EnemiesLeft++;
}

// Will be called by enemies
void AArenaManager::DecrementEnemies()
{
	EnemiesLeft--;
	UE_LOG(LogTemp, Warning, TEXT("Enemies Left: %i"), EnemiesLeft)

	if (EnemiesLeft <= 0)
	{
		int32 Counter{0};

		// Spawn next wave for each spawner
		for (int32 i{ 0 }; i < AmountOfSpawners; i++)
		{
			EnemySpawners[i]->SpawnNextWave();

			// If the spawner had no one to spawn this wave
			if (EnemySpawners[i]->bEmptyWave == true) Counter++;
		}

		// If all of the spawners had an empty wave (no one to spawn), then consider encounter complete.
		if (Counter == AmountOfSpawners)
		{
			UE_LOG(LogTemp, Warning, TEXT("COMPLETE"))
			TimePassed = 0.0f;
			bRaisePlatforms = true;
			bLowerPlatforms = false; // Make it possible to exit encounter area
		}
	}

}

void AArenaManager::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Starting encounter ..."))

	// Start spawning enemies
	for (int32 i{ 0 }; i < AmountOfSpawners; i++)
	{
		EnemySpawners[i]->SpawnNextWave();
	}

	// Force player to fight enemies by lowering platforms
	TimePassed = 0.0f;
	bRaisePlatforms = false;
	bLowerPlatforms = true;

	// After only one detection, the collider will destroy itself
	EnterArenaCollider->DestroyComponent();
}
