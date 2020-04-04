// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "Herder.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Initialize Object Components

	// HerderColliders is used as an empty component just used as a parent of behind/left/right colliders
	HerderAI = CreateDefaultSubobject<USceneComponent>(TEXT("HerderAI"));
	HerderAI->SetupAttachment(GetRootComponent());

	// Used in the same manner as HerderAI. Just used for arranging the hierarchy
	ParentOfPoints = CreateDefaultSubobject<USceneComponent>(TEXT("ParentOfPoints"));
	ParentOfPoints->SetupAttachment(HerderAI);

	FlashLightCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("FlashLightCollider"));
	FlashLightPivot = CreateDefaultSubobject<USphereComponent>(TEXT("FlashLightColliderPivot"));

	LaserCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("LaserCollider"));
	LaserCollider->SetRelativeScale3D(FVector(90.f, 1.5f, 1.5f));
	LaserCollider->SetRelativeLocation(FVector(2920.f, 0.f, 0.f));

	LaserPivot = CreateDefaultSubobject<USphereComponent>(TEXT("LaserColliderPivot"));
	LaserPivot->SetRelativeScale3D(FVector(.25f, .25f, .25f));
	LaserPivot->SetRelativeLocation(FVector(40, 0.f, 0.f));


	// Colliders for Herder AI Behvaiour
	BehindCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BehindCollider"));
	BehindCollider->SetupAttachment(HerderAI);
	BehindCollider->SetRelativeLocation(FVector(-750.0f, 0.0f, 40.0f));
	BehindCollider->SetBoxExtent(FVector(750.0f, 700.0f, 80.0f));

	LeftCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("LetCollider"));
	LeftCollider->SetupAttachment(HerderAI);
	LeftCollider->SetRelativeLocation(FVector(0.0f, -800.0f, 40.0f));
	LeftCollider->SetBoxExtent(FVector(1500.0f, 800.0f, 80.0f));

	RightCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("RightCollider"));
	RightCollider->SetupAttachment(HerderAI);
	RightCollider->SetRelativeLocation(FVector(0.0f, 800.0f, 40.0f));
	RightCollider->SetBoxExtent(FVector(1500.0f, 800.0f, 80.0f));

	MidCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("MidCollider"));
	MidCollider->SetupAttachment(HerderAI);
	MidCollider->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
	MidCollider->SetBoxExtent(FVector(150.0f, 150.0f, 80.0f));

	// Left point
	LeftPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftPoint"));
	LeftPoint->SetupAttachment(ParentOfPoints);
	LeftPoint->SetRelativeRotation(FRotator(0.0f, 230.0f, 0.0f));
	LeftPoint->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Right point
	RightPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightPoint"));
	RightPoint->SetupAttachment(ParentOfPoints);
	RightPoint->SetRelativeRotation(FRotator(0.0f, -230.0f, 0.0f));
	RightPoint->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Distance variables for Left/Right point
	LeftDistance = 700.0f;
	RightDistance = 700.0f;

	// VALUES
	LengthOfTrace = 700.0f;
	DirectionOfTrace = FVector(0.0f, 0.0f, -1.0f);
	TraceParams = new FCollisionQueryParams();
	HitResult = new FHitResult();
	RightValid = true;
	LeftValid = true;

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

	// Points behind player spawned at different locations (affected by intial this->ActorRotation)
	// Because I set their location further down as 'SetRelativeLocation', they spawned at twice the
	// rotation of GetActorRotation(). To fix this, I made the parent of these points half the rotation
	// of GetActorRotation(). This will spawn them at the intended location.
	float Yaw{ GetActorRotation().Yaw / 2};
	ParentOfPoints->SetWorldRotation(FRotator(0.0f, Yaw, 0.0f));

	// Forward vectors of left/right point
	LeftForward = LeftPoint->GetForwardVector();
	RightForward = RightPoint->GetForwardVector();

	// Set left/right point at correct distance/locations
	LeftPoint->SetRelativeLocation(LeftForward * LeftDistance);
	RightPoint->SetRelativeLocation(RightForward * RightDistance);

	// Start looking for OverlapEvents in the following colliders:
	BehindCollider->OnComponentEndOverlap.AddDynamic(this, &AMyPlayer::BehindOverlapEnd);
	LeftCollider->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::LeftOverlapBegin);
	RightCollider->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::RightOverlapBegin);
	MidCollider->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::MidOverlapBegin);

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

	// Always set the points to be the furthest distance possible (closest to 700)
	// This distance will be reduced to a valid distance in the function ReduceDistance();
	LeftDistance = 700.0f;
	RightDistance = 700.0f;
	LeftPoint->SetRelativeLocation(LeftForward * LeftDistance);
	RightPoint->SetRelativeLocation(RightForward * RightDistance);

	// Reduce the distance from Player to Left/Right Point to a valid distance
	ReduceDistance();

	// Update these values which will be sent to the AI for its behaviour to work
	LeftPointLocation = LeftPoint->GetComponentLocation();
	RightPointLocation = RightPoint->GetComponentLocation();

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


void AMyPlayer::BehindOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AHerder* Herder;
		Herder = Cast<AHerder>(OtherActor);

		if (Herder)
		{
			if (Herder->bStageOneComplete)
			{
				Herder->bStageOneComplete = false;
			}
		}
	}
}

void AMyPlayer::LeftOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AHerder* Herder;
		Herder = Cast<AHerder>(OtherActor);

		if (Herder)
		{
			Herder->bPrioritizeLeft = true;
		}
	}
}

void AMyPlayer::RightOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AHerder* Herder;
		Herder = Cast<AHerder>(OtherActor);

		if (Herder)
		{
			Herder->bPrioritizeLeft = false;
		}
	}
}

void AMyPlayer::MidOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AHerder* Herder;
		Herder = Cast<AHerder>(OtherActor);

		if (Herder)
		{
			Herder->bPrioritizePlayer = true;
		}
	}
}

void AMyPlayer::ReduceDistance()
{
	CheckLeftValid();
	CheckRightValid();

	while (!LeftValid)
	{
		// Smaller decrements will reduce performance drastically, but will increase accuracy.
		// If you set the points to be visible in-game, you can see what I mean.
		// If the points were supposed to be visible to the player, I would've figured out
		// a more efficient, smoother solution.
		LeftDistance -= 60.0f;
		LeftPoint->SetRelativeLocation(LeftForward * LeftDistance);
		CheckLeftValid();
		if (LeftDistance <= 70.0f) break;
	}

	while (!RightValid)
	{
		// Smaller decrements will reduce performance drastically, but will increase accuracy.
		// If you set the points to be visible in-game, you can see what I mean.
		// If the points were supposed to be visible to the player, I would've figured out
		// a more efficient, smoother solution.
		RightDistance -= 60.0f;
		RightPoint->SetRelativeLocation(RightForward * RightDistance);
		CheckRightValid();
		if (RightDistance <= 70.0f) break;
	}
}

void AMyPlayer::CheckLeftValid()
{
	FVector LeftStart = LeftPoint->GetComponentLocation();
	FVector LeftEnd = LeftStart + (LengthOfTrace * DirectionOfTrace);

	// Raycast from LeftPoint
	if (GetWorld()->LineTraceSingleByChannel(*HitResult, LeftStart, LeftEnd, ECC_Visibility, *TraceParams))
	{
		//DrawDebugLine(GetWorld(), LeftStart, LeftEnd, FColor(255, 0, 0), true);
		// If the object hit has tag "encounter", return valid. Else return false
		LeftValid = (HitResult->Actor->ActorHasTag("encounter")) ? true : false;
	}
}

void AMyPlayer::CheckRightValid()
{
	FVector RightStart = RightPoint->GetComponentLocation();
	FVector RightEnd = RightStart + (LengthOfTrace * DirectionOfTrace);

	// Raycast from RightPoint
	if (GetWorld()->LineTraceSingleByChannel(*HitResult, RightStart, RightEnd, ECC_Visibility, *TraceParams))
	{
		//DrawDebugLine(GetWorld(), RightStart, RightEnd, FColor(255, 0, 0), true);
		// If the object hit has tag "encounter", return valid. Else return false
		RightValid = (HitResult->Actor->ActorHasTag("encounter")) ? true : false;
	}
}