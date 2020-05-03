// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnklebiterAnimInstance.generated.h"

//Taken from the UDEMY course
/**
 *
 */
UCLASS()
class LIGHT_PROTOTYPE_API UAnklebiterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable)
		void UpdateAnimationProperties();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bisInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class APawn* Pawn;


};
