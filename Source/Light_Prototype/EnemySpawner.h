// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enemy.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"


// ENUM identical to the ENUM in 'Enemy.h' where the these spawners will check if an enemy instance
// is assigned the same integer value (same label). If identical: the enemy should be spawned by this spawner.
UENUM(BlueprintType)
enum class ESpawnLabel : uint8
{
	ESL_None	    UMETA(Displayname = "None"),
	ESL_Label_One	UMETA(Displayname = "Label_One"),
	ESL_LABEL_Two   UMETA(Displayname = "Label_Two"),
	ESL_Label_Three UMETA(Displayname = "Label_Three"),
	ESL_Label_Four  UMETA(Displayname = "Label_Four"),
	ESL_Max		    UMETA(Displayname = "DefaultMax")
};


UCLASS()
class LIGHT_PROTOTYPE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemySpawner();

	UPROPERTY(EditAnywhere, Category = "Visual of Points")
	class UStaticMeshComponent* EndPoint;

	UPROPERTY(EditAnywhere, Category = "Visual of Points")
	class UStaticMeshComponent* StartPoint;

	// What group of enemies this spawner should spawn. Assignment is made in editor.
	UPROPERTY(EditAnywhere, Category = "Enums")
	ESpawnLabel SpawnLabel;

	// Array filled up with instances to spawn
	TArray<AEnemy*> Enemies;

	// TimerHandle to track time processes
	FTimerHandle MyTimerHandle;

	int32 MaxIndex;

	int32 CurrentWave;

	int32 CurrentIndex;

	// Spawners are limited to 3 waves for now...
	UPROPERTY(EditAnywhere, Category = "Waves")
	int32 Waves[3];

	float Seconds; // ONLY DEBUGGING!

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Spawn next enemy in line
	void SpawnEnemies();

	void SpawnNextWave();

};
