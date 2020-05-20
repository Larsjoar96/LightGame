// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserBeamVisual.generated.h"

UCLASS()
class LIGHT_PROTOTYPE_API ALaserBeamVisual : public AActor
{
	GENERATED_BODY()
	
public:	
	ALaserBeamVisual();

	// Mesh
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* LaserLightVisual;

	// Lights to light up scene around laser
	UPROPERTY(EditAnywhere)
	class URectLightComponent* OverLight;
	UPROPERTY(EditAnywhere)
	class URectLightComponent* UnderLight;

	class AMyPlayer* PlayerRef;

private:
	class UMaterialInstanceDynamic* LaserMaterial;

	float InitialLightBrightness;
	float NewLightBrightness;
	float TimePassed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
