// Fill out your copyright notice in the Description page of Project Settings.


#include "Ankelbiter.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"


void AAnkelbiter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Weakening feedback, color transition
	// Temporarily fix for the additional material in Ankelbiters. In final game, all enemies will only have one
	// material. So the function found in "Enemy.cpp" is enough
	UMaterialInstanceDynamic* TempMaterial = Super::GetMesh()->CreateDynamicMaterialInstance(1);
	if (TempMaterial /*&& !bIsStunned*/)
	{
		TempMaterial->SetScalarParameterValue(FName("WeakenedAmount"), (TimeInFlashlight / TimeUntilStunned));
	}
}