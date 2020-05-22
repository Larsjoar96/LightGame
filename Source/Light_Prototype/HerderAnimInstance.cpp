// Fill out your copyright notice in the Description page of Project Settings.


#include "HerderAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy.h"

void UHerderAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
}

void UHerderAnimInstance::UpdateAnimationProperties()
{
	if(Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if(Pawn)
	{
		bisInAir = Pawn->GetMovementComponent()->IsFalling();
		
		EnemyPtr = Cast<AEnemy>(Pawn);

		if (EnemyPtr->IsA<AEnemy>())
		{
			bEnemyDead = EnemyPtr->bDead;
		}
	}
}