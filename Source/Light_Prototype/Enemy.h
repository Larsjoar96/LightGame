// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


// ENUM identical to the ENUM found in 'EnemySpawner.h'
// ENUM used to identify which 'EnemySpawner' instance this particular enemy is assigned.
UENUM(BlueprintType)
enum class EEnemyLabel : uint8
{
	ESL_None 	    UMETA(Displayname = "None"),
	ESL_Label_One	UMETA(Displayname = "Label_One"),
	ESL_LABEL_Two   UMETA(Displayname = "Label_Two"),
	ESL_Label_Three UMETA(Displayname = "Label_Three"),
	ESL_Label_Four  UMETA(Displayname = "Label_Four"),
	ESL_Max		    UMETA(Displayname = "DefaultMax")
};


UCLASS()
class LIGHT_PROTOTYPE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Assign which 'EnemySpawner' this enemy should spawn at
	UPROPERTY(EditAnywhere, Category = "MyVariables | Enums")
	EEnemyLabel EnemyLabel;

	// ArenaDetector is communicating with its ArenaManager regarding OverlapEvents
	UPROPERTY(EditAnywhere, Category = "Collision")
	class UBoxComponent* ArenaDetector;

	UPROPERTY(EditAnywhere, Category = "Collision")
	class UBoxComponent* FlashlightDetector;

	UPROPERTY(EditAnywhere, Category = "Collision")
	class UBoxComponent* LaserDetector;

	UPROPERTY()
	class AMyPlayer* Player;

	UPROPERTY()
	class ACharacter* CharacterCaster;

	UPROPERTY()
	bool bBeingStunned;//Check if enemy is standing in flashlight

	UPROPERTY()
	bool bIsStunned;//Check if enemy is stunned 

	bool bDead;

	// If enemy is NOT initialized in spawn pool. If this enemy is placed in the game world by a level designer.
	UPROPERTY(EditAnywhere, Category = "MyVariables | PreSpawner")
	bool bPreSpawnedEnemy;

	UPROPERTY()
	float TimeStunned;//How long the enemy should be stunned in seconds

	UPROPERTY()//How long it takes to stun enemy in seconds
	float TimeUntilStunned;

	UPROPERTY()
	float TimeInFlashlight;//How long the enemy has been isnide flashlight

	UPROPERTY()
	float HerderTopSpeed;// Herder max walk speed

	UPROPERTY()
	float AnkelbiterTopSpeed;// Ankelbiter max walk speed

	UPROPERTY()
	float MoveAttackCollider;

	UPROPERTY()
	float MovementSpeedReduction;//Decides how much speed enemy should lose by being inside flashlight

	UPROPERTY()
	FVector SpawnPoolLocation;//Location of where non-used enemies will be




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void ArenaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ArenaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void FlashLightBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void FlashLightEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void LaserBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void LaserEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void AttackRangeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AttackRangeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void Stunning();

	UFUNCTION()
	void Rally();

	UFUNCTION()
	void Die();




};