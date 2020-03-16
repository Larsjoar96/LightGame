// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaManager.generated.h"


UCLASS()
class LIGHT_PROTOTYPE_API AArenaManager : public AActor
{
	GENERATED_BODY()
	
	// ArenaManager is used per enemy encounter platform.
	// Will keep count of amount of enemies left and other important values.

public:	
	// Sets default values for this actor's properties
	AArenaManager();

	// Volume to keep count of enemies
	UPROPERTY(EditAnywhere, Category = "Arena")
	class UBoxComponent* ArenaVolume;

	// Collider to detect when player enters the encounter platform
	UPROPERTY(EditAnywhere, Category = "Arena")
	class UBoxComponent* EnterArenaCollider;

	// Platforms which should be lowered/raised when entering the arena
	UPROPERTY(EditAnywhere, Category = "Arena")
	class UStaticMeshComponent* EnterPlatform;
	UPROPERTY(EditAnywhere, Category = "Arena")
	class UStaticMeshComponent* ExitPlatform;;

	// Fill up array with the spawners which should be affected by this manager
	UPROPERTY(EditAnywhere, Category = "EnemySpawners")
	TArray<class AEnemySpawner*> EnemySpawners;

	int32 EnemiesLeft;

	int32 AmountOfSpawners;

	float TimePassed;

	// Calculated Z-values for lowering and raising enter/exit platforms
	float LoweringZ, RaisingZ;

	bool bEncounterComplete;

	bool bLowerPlatforms;

	bool bRaisePlatforms;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function will be called whenever an enemy begins collision with 'ArenaVolume'
	void IncrementEnemies();

	// Function will be called whenever an enemy ends collision with 'ArenaVolume'
	void DecrementEnemies();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
