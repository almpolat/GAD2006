#pragma once

#include "CoreMinimal.h"
#include "NetBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NetAvatar.generated.h"

UCLASS()
class LABWORK4_API ANetAvatar : public ANetBaseCharacter
{
	GENERATED_BODY()

public:
	ANetAvatar();

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite)
	float MovementScale;

private:
	void MoveForward(float Scale);
	void MoveRight(float Scale);
};