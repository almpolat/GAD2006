#pragma once

#include "CoreMinimal.h"
#include "NetBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "NetAvatar.generated.h"

UCLASS()
class LABWORK4_API ANetAvatar : public ANetBaseCharacter
{
    GENERATED_BODY()

public:
    ANetAvatar();

    UPROPERTY(EditAnywhere)
    UCameraComponent* Camera;

    UPROPERTY(EditAnywhere)
    USpringArmComponent* SpringArm;

    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(BlueprintReadWrite)
    float MovementScale;

    UPROPERTY(ReplicatedUsing = OnRep_bRunning, BlueprintReadWrite)
    bool bRunning;

    UFUNCTION()
    void OnRep_bRunning();

private:
    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void StartRun();
    void StopRun();
};