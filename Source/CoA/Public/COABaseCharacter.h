#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "COABaseCharacter.generated.h"

UCLASS()
class COA_API ACOABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
	float Stamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
	float StaminaGainRate = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
	float StaminaDrainRate = 20.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "COA")
	bool bStaminaDrained = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "COA")
	bool bRunning = false;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;	

};