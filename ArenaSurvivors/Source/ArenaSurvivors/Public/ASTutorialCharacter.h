#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ASTutorialCharacter.generated.h"

UCLASS()
class ARENASURVIVORS_API AASTutorialCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AASTutorialCharacter();

    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent) override;

    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* FollowCamera;

protected:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void TurnRate(float Value);
};