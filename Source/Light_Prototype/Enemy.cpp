// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Pickup_LaserWidener.h"
#include "Pickup_FasterReload.h"
#include "FlashlightWidener.h"
#include "Engine/World.h"
#include "ArenaManager.h"
#include "TimerManager.h"
#include "Ankelbiter.h"
#include "MyPlayer.h"
#include "Herder.h"
#include "Pickup.h"

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

	// Create component for Attack Range Collider
	// Attributes will be different for each enemy, so box extent etc. will be modified in BP
	AttackRange = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackRange"));
	AttackRange->SetupAttachment(GetRootComponent());
	AttackRange->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackRange->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// Create component for Start Attacking Range Collider
	// Attributes will be different for each enemy, so box extent etc. will be modified in BP
	StartAttackingRange = CreateDefaultSubobject<UBoxComponent>(TEXT("StartAttackingRange"));
	StartAttackingRange->SetupAttachment(GetRootComponent());
	StartAttackingRange->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	StartAttackingRange->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// Default 'none' value, as a safety if you forgot to assign it in the editor
	EnemyLabel = EEnemyLabel::ESL_None;

	TimeStunned = 3;
	TimeUntilStunned = 2;
	TimeInFlashlight = 0;
	HerderTopSpeed = 350.0f;
	AnkelbiterTopSpeed = 250.0f;
	MovementSpeedReduction = 60;
	bRotateTowardsPlayer = true;
	bPreSpawnedEnemy = false;
	bWithinAttackRange = false;
	bAttacking = false;
	bDead = true;

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
	
	AttackRange->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AttackRangeEndOverlap);

	StartAttackingRange->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::StartAttackingRangeBeginOverlap);

	if (bPreSpawnedEnemy == false)
	{
		SpawnPoolLocation = GetActorLocation();
	}
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

	// Adding gradient effect to material for feedback to player, regarding weakening progression.
	// VERY IMPORTANT: when the real materials are imported and used, change the material from inside the node in 'MEnemyDefault'. 
	// Don't swap the whole material class itself. This is to still keep the blueprint functionalities within the material.
	UMaterialInstanceDynamic* EnemyMaterial = GetMesh()->CreateDynamicMaterialInstance(0);
	if (EnemyMaterial /*&& !bIsStunned*/)
	{
		EnemyMaterial->SetScalarParameterValue(FName("WeakenedAmount"), (TimeInFlashlight / TimeUntilStunned));
	}	

	// If enemy is attacking, the enemy should not be able to move
	if (bAttacking) GetCharacterMovement()->MaxWalkSpeed = 0;
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
	if (OtherComp->IsA<UStaticMeshComponent>())
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
	if (OtherComp->IsA<UStaticMeshComponent>())
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


void AEnemy::AttackRangeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AMyPlayer>(OtherActor))
	{
		bWithinAttackRange = false;
	}
}

void AEnemy::StartAttackingRangeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AMyPlayer>(OtherActor))
	{
		bWithinAttackRange = true;
		if (!bIsStunned)
		{
			Attack();
		}
	}
}


void AEnemy::Stunning()
{
	if (TimeInFlashlight >= TimeUntilStunned)
	{
		bIsStunned = true;
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
	else if (!bIsStunned)
	{
		if (Cast<AHerder>(this))
		{
			GetCharacterMovement()->MaxWalkSpeed = HerderTopSpeed - (TimeInFlashlight * MovementSpeedReduction);
		}
		else if (Cast<AAnkelbiter>(this))
		{
			GetCharacterMovement()->MaxWalkSpeed = AnkelbiterTopSpeed - (TimeInFlashlight * MovementSpeedReduction);
		}
	}
}

void AEnemy::Rally()
{
	bIsStunned = false;
	if (!bDead)
	{
		if (Cast<AHerder>(this))
		{
			GetCharacterMovement()->MaxWalkSpeed = HerderTopSpeed;
		}
		else if (Cast<AAnkelbiter>(this))
		{
			GetCharacterMovement()->MaxWalkSpeed = AnkelbiterTopSpeed;
		}
	}
}

void AEnemy::Die()
{
	if (bIsStunned)
	{
		if (bPreSpawnedEnemy == false)
		{
			int32 PowerUpIndex{};

			// Make enemy drop PowerUp number 'PowerUpIndex'
			PowerUpIndex = UKismetMathLibrary::RandomIntegerInRange(1, 3);
			SpawnPowerUp(PowerUpIndex);

			bDead = true;
			bRotateTowardsPlayer = false;
			GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &AEnemy::TpEnemyToPool, 0.930f);
			GetCharacterMovement()->MaxWalkSpeed = 0.0f;
		}
		else
		{
			Destroy();
		}
	}
}

void AEnemy::TpEnemyToPool()
{
	SetActorLocation(SpawnPoolLocation);
	TimeInFlashlight = 0;
	bRotateTowardsPlayer = true;
}

void AEnemy::Attack()
{
	bAttacking = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(FName("Attack_1"), AttackMontage);
	}
}

void AEnemy::EndAttack()
{
	bAttacking = false;

	// If still within attack range when attack ends, continue attacking
	if (bWithinAttackRange)
	{
		Attack();
	}
}

void AEnemy::DamagePlayer()
{
	if (bWithinAttackRange)
	{
		AMyPlayer* PlayerRef = Cast<AMyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		PlayerRef->PlayerHealth -= 1;
	}
}

void AEnemy::SpawnPowerUp(int32 PowerUpIndex)
{
	TSubclassOf<APickup> PowerUpToSpawn;
	float Random{};

	if (FasterReload && FlashlightWidener && LazerWidener)
	{
			switch (PowerUpIndex)
			{
			case 1:
				PowerUpToSpawn = FasterReload;
				UE_LOG(LogTemp, Warning, TEXT("FasterReload selected"))
				break;
			case 2:
				PowerUpToSpawn = FlashlightWidener;
				UE_LOG(LogTemp, Warning, TEXT("FlashlightWidener selected"))
				break;
			case 3:
				PowerUpToSpawn = LazerWidener;
				UE_LOG(LogTemp, Warning, TEXT("LazerWidener selected"))
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("PowerUpIndex's range exceeds the amount of existing PowerUps"))
				// Quit game function
				break;
			}

		Random = UKismetMathLibrary::RandomFloat();

		// Adjust drop percentage here
		if (Random <= 0.10f)
		{
			FActorSpawnParameters SpawnParams;
			FVector Location = GetActorLocation() + FVector(0.0f, 0.0f, 20.0f);
			FRotator Rotation(0.0f);

			// Spawn the 'PowerUpToSpawn' when enemy dies
			APickup* PowerUpRef = GetWorld()->SpawnActor<APickup>(PowerUpToSpawn, Location, Rotation, SpawnParams);
			UE_LOG(LogTemp, Warning, TEXT("BINGO! You rolled a %f!"), Random)
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unlucky, you rolled a %f"), Random)
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Enemies are missing PowerUp! Assign the corresponding PowerUp in Herder/Ankelbiter BP ..."))
	}


}