// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Initialize Object Components

	FlashLightCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("FlashLightCollider"));
	FlashLightPivot = CreateDefaultSubobject<USphereComponent>(TEXT("FlashLightColliderPivot"));
	LaserCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("LaserCollider"));
	LaserCollider->SetRelativeScale3D(FVector(90.f, 1.5f, 1.5f));
	LaserCollider->SetRelativeLocation(FVector(2920.f, 0.f, 0.f));
	LaserPivot = CreateDefaultSubobject<USphereComponent>(TEXT("LaserColliderPivot"));
	LaserPivot->SetRelativeScale3D(FVector(.25f, .25f, .25f));
	LaserPivot->SetRelativeLocation(FVector(40, 0.f, 0.f));


	//PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));

	FlashLightCollider->SetupAttachment(FlashLightPivot);
	LaserCollider->SetupAttachment(LaserPivot);
	LaserPivot->SetupAttachment(GetRootComponent());
	FlashLightPivot->SetupAttachment(GetRootComponent());

	ColliderLocationOffset = 9000.0f;//Used for hiding flashlight cone and laser

	//Initialize Flashlight collider Transforms
	LightLocationDefault = FlashLightPivot->GetRelativeLocation();
	LightScaleDefault = FlashLightPivot->GetRelativeScale3D();
	LightRotationDefault = this->GetActorRotation();

	FlashLightScaleModifier = 1.1f;
	LightReduceScaleMod = 1.01f;//Used to make the scale of the flashlight go down when you charge up laser
	LightReturnSpeed = 1.0f; //Determines how fast you flashlight returns after shooting your laser

	//Initialize Laser Collider Transforms
	LaserLocationDefault = LaserPivot->GetRelativeLocation();
	LaserLocationDefault = FVector(LaserLocationDefault.X, LaserLocationDefault.Y, LaserLocationDefault.Z - ColliderLocationOffset);
	LaserScaleDefault = LaserPivot->GetRelativeScale3D();
	LaserRotationDefault = this->GetActorRotation();

	//Initialize PowerUp Variables
	PowerUpTime = 45.0f;
	PowerUpTimeLeft = 0;
	ReloadSpeedDefault = 1.0f;
	ReloadSpeedCurrent = 1.0f;
	ReloadSpeedUpgraded = 4.0f;
	PowerUpLightScale = 2.0f;
	PowerUpLaserScale = 2.0f;

	//Combat values
	PlayerHealth = 5;
	DamageTaken = 1;
	ShootingTime = 0.2f;



}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	//Initialize Flashlight collider Transforms
	LightLocationCurrent = LightLocationDefault;
	LightScaleCurrent = LightScaleDefault;
	LightRotationCurrent = LightRotationDefault;

	LightLocationPoweredUp = LightLocationDefault;
	LightScalePoweredUp = FVector(LightScaleDefault.X, LightScaleDefault.Y * PowerUpLightScale, LightScaleDefault.Z * PowerUpLightScale);
	LightRotationPoweredUp = LightRotationDefault;


	//Initialize Laser Collider Transforms
	LaserLocationCurrent = LaserLocationDefault;
	LaserScaleCurrent = LaserScaleDefault;
	LaserRotationCurrent = LaserRotationDefault;

	LaserLocationPoweredUp = LaserLocationDefault;
	LaserScalePoweredUp = FVector(LaserScaleDefault.X, LaserScaleDefault.Y * PowerUpLaserScale, LaserScaleDefault.Z * PowerUpLaserScale);
	LaserRotationPoweredUp = LaserRotationDefault;


	LaserPivot->SetRelativeLocation(LaserLocationDefault);
	LaserPivot->SetRelativeRotation(LaserRotationDefault);
	FlashLightPivot->SetRelativeLocation(LightLocationDefault);
	FlashLightPivot->SetRelativeRotation(LightRotationDefault);

}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Time = DeltaTime;

	FlashLightPivot->SetRelativeScale3D(FVector(LightScaleCurrent.X, (LightScaleCurrent.Y * (LightReduceScaleMod - LaserCharger)), LightScaleCurrent.Z));

	LightBehaviour();
	if (bHasPowerUp == true)
	{
		PowerUpTimeLeft = PowerUpTimeLeft - Time;
		if (PowerUpTimeLeft <= 0)
		{
			LosePowerup();
		}
	}
	if (bJustTookDamage == true)
	{
		TimeRecovering = TimeRecovering + Time;
		if (TimeRecovering >= TimeToRecover)
		{
			bJustTookDamage = false;
			//End damagefeedback here....
			TimeRecovering = 0;
		}
	}

}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMyPlayer::ChargeUp);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AMyPlayer::Shoot);
}

void AMyPlayer::ChargeUp()//Hold button to charge 
{
	if (bJustShot == false)
	{
		bIsCharging = true;
	}
}

void AMyPlayer::Shoot()//Shoot if your laser is fully charged
{
	if (bJustShot == false)
		UE_LOG(LogTemp, Warning, TEXT("Shoot!"));
	{
		if (LaserCharger >= LaserFullyCharged)
		{
			UE_LOG(LogTemp, Warning, TEXT("PEW!"));

			LaserPivot->SetRelativeLocation(FVector(LaserLocationDefault.X,LaserLocationDefault.Y, LaserLocationDefault.Z + ColliderLocationOffset));	//Set location of Laser hitbox ahead of the player

			GetWorld()->GetTimerManager().SetTimer(MyTimerHandle, this, &AMyPlayer::CooledDown, (ShootingTime));


		}
		else
		{
			bIsCharging = false;
			FlashLightPivot->SetRelativeLocation(LightLocationDefault);

		}
	}
}

void AMyPlayer::PickupEventBiggerFlashlight()//Increase size of flashlight
{
	LosePowerup();
	LightLocationCurrent = LightLocationPoweredUp;
	LightScaleCurrent = LightScalePoweredUp;
	LightRotationCurrent = LightRotationPoweredUp;
	FlashLightPivot->SetRelativeScale3D(LightScaleCurrent);

	bHasPowerUp = true;
	PowerUpTimeLeft = PowerUpTime;

}

void AMyPlayer::PickupEventFasterReload()//Increase fire rate of your finishing move
{
	LosePowerup();
	ReloadSpeedCurrent = ReloadSpeedUpgraded;
	bHasPowerUp = true;
	PowerUpTimeLeft = PowerUpTime;
}

void AMyPlayer::PickupEventBiggerLaser()//Increase the size of the finishing move
{
	LosePowerup();
	LaserLocationCurrent = LaserLocationPoweredUp;
	LaserScaleCurrent = LaserScalePoweredUp;
	LaserRotationCurrent = LaserRotationPoweredUp;
	LaserPivot->SetRelativeScale3D(LaserScaleCurrent);

	bHasPowerUp = true;
	PowerUpTimeLeft = PowerUpTime;
}

void AMyPlayer::LosePowerup()//Remove the power you have and reset the timer(Not finished!!!)
{
	LightLocationCurrent = LightLocationDefault;
	LightScaleCurrent = LightScaleDefault;
	LightRotationCurrent = LightRotationDefault;

	FlashLightPivot->SetRelativeLocation(LightLocationDefault);
	FlashLightPivot->SetRelativeRotation(LightRotationDefault);
	FlashLightPivot->SetRelativeScale3D(LightScaleDefault);


	LaserScaleCurrent = LaserScaleDefault;
	LaserRotationCurrent = LaserRotationDefault;

	LaserPivot->SetRelativeRotation(LaserRotationDefault);
	LaserPivot->SetRelativeScale3D(LaserScaleDefault);

	ReloadSpeedCurrent = ReloadSpeedDefault;

	bHasPowerUp = false;

}

void AMyPlayer::TakeDamageTho()
{
	PlayerHealth = PlayerHealth - DamageTaken;

	if (PlayerHealth <= 0)
	{
		this -> Destroy();
	}
	else
	{
		//Damage feedback and invinsibility frames
	}
}

void AMyPlayer::LightBehaviour()
{
	if (bIsCharging == true)
	{
		if (LaserCharger >= LaserFullyCharged) //Are you fully charged ?
		{
			FlashLightPivot->SetRelativeLocation(FVector(LightLocationDefault.X, LightLocationDefault.Y, LightLocationDefault.Z - ColliderLocationOffset));//HideFlashlight

		}
		if (LaserCharger < LaserFullyCharged)
		{
			LaserCharger = LaserCharger + (ReloadSpeedCurrent * Time);
		}
		FlashLightPivot->SetRelativeScale3D(FVector(LightScaleCurrent.X, (LightScaleCurrent.Y * (LightReduceScaleMod - LaserCharger)), LightScaleCurrent.Z));
	}
	else
	{

		if (bJustShot == true)
		{
			LaserCharger = LaserCharger - (LightReturnSpeed * (ReloadSpeedCurrent * Time));

			if (LaserCharger <= 0)
			{
				bJustShot = false;
				FlashLightPivot->SetHiddenInGame(false);
				//Set location of Finishing move to hidden
				LaserPivot->SetRelativeLocation(LaserLocationDefault);
				//Set location of flashlight hitbox in front of player
				FlashLightPivot->SetRelativeLocation(LightLocationDefault);
				//Change scale of Flashlight hitbox light
				FlashLightPivot->SetRelativeScale3D(FVector(LightScaleCurrent.X, (LightScaleCurrent.Y * (LightReduceScaleMod - LaserCharger)), LightScaleCurrent.Z));
			}
			else
			{
				//Flicker flashlight
				if (FlashLightPivot->bHiddenInGame == true)
				{
					FlashLightPivot->SetHiddenInGame(false);
				}
				else if(FlashLightPivot->bHiddenInGame == false)
				{
					FlashLightPivot->SetHiddenInGame(true);
				}

				FlashLightPivot->SetRelativeScale3D(FVector(LightScaleCurrent.X, (LightScaleCurrent.Y * (LightReduceScaleMod - LaserCharger)), LightScaleCurrent.Z));
			}
		}
		else
		{
			if (LaserCharger > 0)
			{
				LaserCharger = LaserCharger - (LightReturnSpeed * (ReloadSpeedCurrent * Time));
				FlashLightPivot->SetRelativeScale3D(FVector(LightScaleCurrent.X, (LightScaleCurrent.Y * (LightReduceScaleMod - LaserCharger)), LightScaleCurrent.Z));
			}
		}
	}

}

void AMyPlayer::LookAtMouse()
{

}

void AMyPlayer::CooledDown()
{
	LaserPivot->SetRelativeLocation(LaserLocationDefault);//Set location of Laser hitbox in hide location

	FlashLightPivot->SetRelativeLocation(LightLocationDefault);//Set location of Flashlight hitbox ahead of the player
	bIsCharging = false;
	bJustShot = true;
}



