// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	//Initialize Object Components
	FlashLightCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("FlashLightCollider"));
	LaserCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("LaserCollider"));
	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));

	ColliderLocationOffset = 9000.0f;
	//Initialize Flashlight collider Transforms
	LightLocationDefault = FlashLightCollider->GetComponentLocation();
	LightScaleDefault = FlashLightCollider->GetComponentScale();
	LightRotationDefault = FlashLightCollider->GetComponentRotation();

	LightLocationCurrent = LightLocationDefault;
	LightScaleCurrent = LightScaleDefault;
	LightRotationCurrent = LightRotationDefault;

	LightLocationPoweredUp = LightLocationDefault;
	LightScalePoweredUp = LightScaleDefault;
	LightRotationPoweredUp = LightRotationDefault;

	LightLocationPoweredUp = LightLocationDefault - ColliderLocationOffset;
	LightScalePoweredUp.Y = LightScalePoweredUp.Y * UpgradeLaserScale;
	LightScalePoweredUp.Z = LightScalePoweredUp.Z * UpgradeLaserScale;

	//Initialize Laser Collider Transforms
	LaserLocationDefault = LaserCollider->GetComponentLocation();
	LaserScaleDefault = LaserCollider->GetComponentScale();
	LaserRotationDefault = LaserCollider->GetComponentRotation();

	LaserLocationCurrent = LaserLocationDefault;
	LaserScaleCurrent = LaserScaleDefault;
	LaserRotationCurrent = LaserRotationDefault;

	LaserLocationPoweredUp = LaserLocationDefault;
	LaserScalePoweredUp = LaserScaleDefault;
	LaserRotationPoweredUp = LaserRotationDefault;

	LaserLocationPoweredUp.Z = LaserLocationDefault.Z - ColliderLocationOffset;//Hide the Laser somewhere it cant collide with enemies
	LaserScalePoweredUp.Y = LaserScalePoweredUp.Y * UpgradeLaserScale;
	LaserScalePoweredUp.Z = LaserScalePoweredUp.Z * UpgradeLaserScale;


	
>>>>>>> parent of cf34ad3... More blueprint conversion
=======
>>>>>>> parent of adf1628... Merge branch 'master' of https://github.com/Larsjoar96/LightGame
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
<<<<<<< HEAD
<<<<<<< HEAD
	
=======


>>>>>>> parent of cf34ad3... More blueprint conversion
=======
	
>>>>>>> parent of adf1628... Merge branch 'master' of https://github.com/Larsjoar96/LightGame
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	Time = DeltaTime;

	LightBehaviour();
	LookAtMouse();
	if(bHasPowerUp == true) 
	{
		PowerUpTimeLeft = PowerUpTimeLeft - DeltaTime;
		if(PowerUpTimeLeft <= 0)
		{
			LosePowerup();
		}
	}
	if(bJustTookDamage == true)
	{
		TimeRecovering = TimeRecovering + DeltaTime;
		if(TimeRecovering >= TimeToRecover)
		{
			bJustTookDamage = false;
			//End damagefeedback here....
			TimeRecovering = 0;
		}
	}

>>>>>>> parent of cf34ad3... More blueprint conversion
=======
>>>>>>> parent of adf1628... Merge branch 'master' of https://github.com/Larsjoar96/LightGame
}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMyPlayer::ChargeUp);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AMyPlayer::Shoot);
}

void AMyPlayer::ChargeUp()//Hold button to charge finishing move
{
	if(bJustShot == false)
	{
		bIsCharging = true;
	}
}

void AMyPlayer::Shoot()//Shoot if your finishing move is fully charged
{
	if(bJustShot == false)
	{
		if(LaserCharger >= LaserFullyCharged) 
		{

			LaserCollider->SetRelativeLocation(LaserLocationPoweredUp);	//Set location of Laser hitbox ahead of the player
			//Delay by shooting time

			LaserCollider->SetRelativeLocation(LaserLocationDefault);//Set location of Laser hitbox in hide location

			FlashLightCollider->SetRelativeLocation(LightLocationDefault);//Set location of Flashlight hitbox ahead of the player
			bIsCharging = false;
			bJustShot = true;
		}
		else
		{
			bIsCharging = false;
			FlashLightCollider->SetRelativeLocation(LightLocationDefault);

		}
	}
}

void AMyPlayer::PickupEventBiggerFlashlight()//Increase size of flashlight
{
	LosePowerup();
	LightLocationCurrent = LightLocationPoweredUp;
	LightScaleCurrent = LightScalePoweredUp;
	LightRotationCurrent = LightRotationPoweredUp;
	FlashLightCollider->SetRelativeScale3D(LightScaleCurrent);

	bHasPowerUp = true;
	PowerUpTimeLeft = PowerUpTime;
	
}

void AMyPlayer::PickupEventFasterReload()//Increase fire rate of your finishing move
{
	LosePowerup();
	ReloadSpeedCurrent = ReloadSpeedUpgraded;
	bHasPowerUp;
	PowerUpTimeLeft = PowerUpTime;
}

void AMyPlayer::PickupEventBiggerLaser()//Increase the size of the finishing move
{
	LosePowerup();
	LaserLocationCurrent = LaserLocationPoweredUp;
	LaserScaleCurrent = LaserScalePoweredUp;
	LaserRotationCurrent = LaserRotationPoweredUp;

	

	bHasPowerUp = true;
	PowerUpTimeLeft = PowerUpTime;
}

void AMyPlayer::LosePowerup()//Remove the power you have and reset the timer(Not finished!!!)
{
	LightLocationCurrent = LightLocationDefault;
	LightScaleCurrent = LightScaleDefault;
	LightRotationCurrent = LightRotationDefault;

	FlashLightCollider->SetRelativeLocation(LightLocationCurrent);
	FlashLightCollider->SetRelativeRotation(LightRotationCurrent);
	FlashLightCollider->SetRelativeScale3D(LightScaleCurrent);


	LaserLocationCurrent = LaserLocationDefault;
	LaserScaleCurrent = LaserScaleDefault;
	LaserRotationCurrent = LaserRotationDefault;

	LaserCollider->SetRelativeLocation(LaserLocationCurrent);
	LaserCollider->SetRelativeRotation(LaserRotationCurrent);
	LaserCollider->SetRelativeScale3D(LaserScaleCurrent);

	ReloadSpeedCurrent = ReloadSpeedDefault;
>>>>>>> parent of cf34ad3... More blueprint conversion
=======
>>>>>>> parent of adf1628... Merge branch 'master' of https://github.com/Larsjoar96/LightGame

}

