// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enemy.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"


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

	//UPROPERTY(EditAnywhere, Category = "Distance")
	//float TravelDistance;

	// Array filled up with instances to spawn
	TArray<AEnemy*> Enemies;

	// TimerHandle to track time processes
	FTimerHandle MyTimerHandle;

	int32 CurrentIndex;

	int32 MaxIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Spawn next enemy in line
	void SpawnEnemies();

};
