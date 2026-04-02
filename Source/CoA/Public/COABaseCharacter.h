// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "COABaseCharacter.generated.h"

UCLASS()
class COA_API ACOABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACOABaseCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "COA")
	bool bDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "COA")
	float Health;

	UPROPERTY(EditAnywhere, Category = "COA")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "COA")
	float HealingRate;

	UPROPERTY(EditAnywhere, Category = "COA")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaGainRate = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaDrainRate = 20.0f;

	bool bStaminaDrained = false;
	bool bRunning = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
