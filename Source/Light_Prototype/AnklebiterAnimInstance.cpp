// Fill out your copyright notice in the Description page of Project Settings.


#include "AnklebiterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnklebiterAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
}

void UAnklebiterAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn)
	{
		bisInAir = Pawn->GetMovementComponent()->IsFalling();
	}
}