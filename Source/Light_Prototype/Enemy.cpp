// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayer.h"
#include "ArenaManager.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create component for Arena detector
	ArenaDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("ArenaDetector"));
	ArenaDetector->SetupAttachment(GetRootComponent());

	//Create component for Flashlight Detector
	FlashlightDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("FlashlightDetector"));
	FlashlightDetector->SetupAttachment(GetRootComponent());

	//Create component for Laser Detector
	LaserDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("LaserDetector"));
	LaserDetector->SetupAttachment(GetRootComponent());


	// Default 'none' value, as a safety if you forgot to assign it in the editor
	EnemyLabel = EEnemyLabel::ESL_None;

	TimeStunned = 3;
	TimeUntilStunned = 3;
	TimeInFlashlight = 0;
	TopSpeed = 300;
	MovementSpeedReduction = 60;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Start looking for OverlapEvents
	ArenaDetector->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::ArenaBeginOverlap);
	ArenaDetector->OnComponentEndOverlap.AddDynamic(this, &AEnemy::ArenaEndOverlap);

	FlashlightDetector->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::FlashLightBeginOverlap);
	FlashlightDetector->OnComponentEndOverlap.AddDynamic(this, &AEnemy::FlashLightEndOverlap);

	LaserDetector->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::LaserBeginOverlap);
	LaserDetector->OnComponentEndOverlap.AddDynamic(this, &AEnemy::LaserEndOverlap);


	SpawnPoolLocation = GetActorLocation();


}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bBeingStunned)
	{
		if (TimeInFlashlight < TimeUntilStunned)
		{
			TimeInFlashlight += DeltaTime;
			Stunning();
		}
		else
		{
			Stunning();
		}
	}
	else
	{
		TimeInFlashlight -= DeltaTime;

		if (TimeInFlashlight <= 0)
		{
			TimeInFlashlight = 0;
			Rally();
		}
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemy::ArenaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AArenaManager>())
	{
		AArenaManager* CurrentArenaManager = Cast<AArenaManager>(OtherActor);
		CurrentArenaManager->IncrementEnemies();
	}
}


void AEnemy::ArenaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AArenaManager>())
	{
		AArenaManager* CurrentArenaManager = Cast<AArenaManager>(OtherActor);
		CurrentArenaManager->DecrementEnemies();
	}
}

void AEnemy::FlashLightBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->IsA<UBoxComponent>())
	{
		if (OtherComp->ComponentHasTag("Flashlight"))//Is it overlapping with flashlight?
		{
			Player = Cast <AMyPlayer>(OtherComp->GetOwner());//Set Player reference to be equal to the player

			if (Player->bJustShot == false)//Check if player just fired a shot
			{
				bBeingStunned = true;

			}

		}
	}
}

void AEnemy::FlashLightEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->IsA<UBoxComponent>())
	{
		if (OtherComp->ComponentHasTag("Flashlight"))//Was it the flashlight that stopped overlapping ?
		{
			bBeingStunned = false;
		}
	}
}

void AEnemy::LaserBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->IsA<UBoxComponent>())
	{
		if (OtherComp->ComponentHasTag("Laser"))//Is it overlapping with flashlight?
		{
			Die();
			Rally();
		}
	}
}

void AEnemy::LaserEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AEnemy::AttackRangeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
void AEnemy::AttackRangeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


void AEnemy::Stunning()
{
	if (TimeInFlashlight >= TimeUntilStunned)
	{
		bIsStunned = true;
		GetCharacterMovement()->MaxWalkSpeed = 0;
		//Function for material change needed
	}
	else if (!bIsStunned)
	{
		GetCharacterMovement()->MaxWalkSpeed = TopSpeed - (TimeInFlashlight * MovementSpeedReduction);
		//Function for material change needed
	}
}

void AEnemy::Rally()
{
	bIsStunned = false;
	GetCharacterMovement()->MaxWalkSpeed = TopSpeed;
	//Function for material change needed
}

void AEnemy::Die()
{
	if (bIsStunned)
	{
		SetActorLocation(SpawnPoolLocation);
	}
}

