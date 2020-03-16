// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


// ENUM identical to the ENUM found in 'EnemySpawner.h'
// ENUM used to identify which 'EnemySpawner' instance this particular enemy is assigned.
UENUM(BlueprintType)
enum class EEnemyLabel : uint8
{
<<<<<<< HEAD
	ESL_None 	    UMETA(Displayname = "None"),
	ESL_Label_One	UMETA(Displayname = "Label_One"),
	ESL_LABEL_Two   UMETA(Displayname = "Label_Two"),
	ESL_Label_Three UMETA(Displayname = "Label_Three"),
	ESL_Label_Four  UMETA(Displayname = "Label_Four"),
	ESL_Max		    UMETA(Displayname = "DefaultMax")
=======
    ESL_None         UMETA(Displayname = "None"),
    ESL_Label_One    UMETA(Displayname = "Label_One"),
    ESL_LABEL_Two   UMETA(Displayname = "Label_Two"),
    ESL_Label_Three UMETA(Displayname = "Label_Three"),
    ESL_Label_Four  UMETA(Displayname = "Label_Four"),
    ESL_Max            UMETA(Displayname = "DefaultMax")
>>>>>>> Continue-porting-PlayerCPP
};


UCLASS()
class LIGHT_PROTOTYPE_API AEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AEnemy();

    // Assign which 'EnemySpawner' this enemy should spawn at
    UPROPERTY(EditAnywhere, Category = "Enums")
        EEnemyLabel EnemyLabel;

    // ArenaDetector is communicating with its ArenaManager about OverlapEvents
    UPROPERTY(EditAnywhere, Category = "Collision")
        class UBoxComponent* ArenaDetector;

	// Assign which 'EnemySpawner' this enemy should spawn at
	UPROPERTY(EditAnywhere, Category = "MyVariables | Enums")
	EEnemyLabel EnemyLabel;

	// ArenaDetector is communicating with its ArenaManager about OverlapEvents
	UPROPERTY(EditAnywhere, Category = "Collision")
	class UBoxComponent* ArenaDetector;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

<<<<<<< HEAD
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
=======
    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
        void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
>>>>>>> Continue-porting-PlayerCPP
