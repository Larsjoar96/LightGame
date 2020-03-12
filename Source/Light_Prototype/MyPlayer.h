// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Math/TransformNonVectorized.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyPlayer.generated.h"

UCLASS()
class LIGHT_PROTOTYPE_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
		float Time;

	UPROPERTY(EditAnywhere, Category = "Collider")
		class UBoxComponent* FlashLightCollider;
	UPROPERTY(EditAnywhere, Category = "Collider")
		class USphereComponent* FlashLightPivot;
	UPROPERTY(EditAnywhere, Category = "Collider")
		class UBoxComponent* LaserCollider;
	UPROPERTY(EditAnywhere, Category = "Collider")
		class USphereComponent* LaserPivot;
	UPROPERTY(EditAnywhere, Category = "Mesh")
		class USkeletalMeshComponent* PlayerMesh;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserCharge")
		bool bIsCharging;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powerups")
		bool bHasPowerUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserCharge")
		bool bJustShot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageFeedback")
		bool bJustTookDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		int32 PlayerHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageFeedback")
		int32 DamageTaken;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightTransforms")
		float FlashLightScaleModifier;


	//Values for powerups
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powerups")
		float PowerUpTime;//How long a power-up should last
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powerups")
		float PowerUpTimeLeft;//How much time there is left of a power-up
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powerups")
		float ReloadSpeedDefault;//How fast you reload without any upgrades
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powerups")
		float ReloadSpeedCurrent;//How fast you are currently reloading
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powerups")
		float ReloadSpeedUpgraded;//How fast you reload when you have the upgraded reload pickup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powerups")
		float UpgradeLightScale;//How much the "increase light size" pickup should increace it by. 2 = double
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powerups")
		float UpgradeLaserScale;//How much the "increase finishing move size" pickup should increace it by. 2 = double

		//Values for charging up your finishingmove
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserCharge")
		float LightReduceScaleMod;//Helps make the flashlight scale down
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserCharge")
		float LightReturnSpeed;//Affects how fast your light returns if you let go before fully charged
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserCharge")
		float LaserCharger;//Charges up finishing move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserCharge")
		float LaserFullyCharged;//When your finishing move is fully charged
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserCharge")
		float ColliderLocationOffset;//When your finishing move is fully charged

		//Values for damage invincibility after taking damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageFeedback")
		float TimeRecovering;//How long you have been in "recovery state"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageFeedback")
		float TimeToRecover;//How long it will take you to lose invincibility from taking damage

		//Transforms for the light cone
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightTransforms")
		FVector LightLocationDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightTransforms")
		FVector LightScaleDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightTransforms")
		FRotator LightRotationDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightTransforms")
		FVector LightLocationCurrent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightTransforms")
		FVector LightScaleCurrent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightTransforms")
		FRotator LightRotationCurrent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightTransforms")
		FVector LightLocationPoweredUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightTransforms")
		FVector LightScalePoweredUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightTransforms")
		FRotator LightRotationPoweredUp;

	//Transforms for the finishing move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserTransforms")
		FVector LaserLocationDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserTransforms")
		FVector LaserScaleDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserTransforms")
		FRotator LaserRotationDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserTransforms")
		FVector LaserLocationCurrent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserTransforms")
		FVector LaserScaleCurrent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserTransforms")
		FRotator LaserRotationCurrent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserTransforms")
		FVector LaserLocationPoweredUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserTransforms")
		FVector LaserScalePoweredUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserTransforms")
		FRotator LaserRotationPoweredUp;

	void PickupEventBiggerFlashlight();
	void PickupEventBiggerLaser();
	void PickupEventFasterReload();
	void LosePowerup();
	void LightBehaviour();
	void TakeDamageTho();
	void Shoot();
	void ChargeUp();
	void LookAtMouse();








};
