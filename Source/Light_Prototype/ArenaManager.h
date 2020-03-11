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

	UPROPERTY(EditAnywhere, Category = "Arena")
	class UBoxComponent* ArenaVolume;

	int32 EnemiesLeft;

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

};
