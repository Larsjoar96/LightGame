// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Steinen.generated.h"

UCLASS()
class LIGHT_PROTOTYPE_API ASteinen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASteinen();

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Collider")
	class UBoxComponent* Collider;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
