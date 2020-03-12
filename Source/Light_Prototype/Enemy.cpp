// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "ArenaManager.h"

// Sets default values
AEnemy::AEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create component for detector
    ArenaDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("ArenaDetector"));
    ArenaDetector->SetupAttachment(GetRootComponent());

    // Default 'none' value, as a safety if you forgot to assign it in the editor
    EnemyLabel = EEnemyLabel::ESL_None;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    // Start looking for OverlapEvents
    ArenaDetector->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
    ArenaDetector->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnOverlapEnd);

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}
// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->IsA<AArenaManager>())
    {
        AArenaManager* CurrentArenaManager = Cast<AArenaManager>(OtherActor);
        CurrentArenaManager->IncrementEnemies();
    }
}


void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor->IsA<AArenaManager>())
    {
        AArenaManager* CurrentArenaManager = Cast<AArenaManager>(OtherActor);
        CurrentArenaManager->DecrementEnemies();
    }
}