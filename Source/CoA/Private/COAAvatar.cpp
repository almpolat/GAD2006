#include "COAAvatar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ACOAAvatar::ACOAAvatar()
{
    PrimaryActorTick.bCanEverTick = true;

    mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    mSpringArm->SetupAttachment(RootComponent);
    mSpringArm->TargetArmLength = 300.0f;
    mSpringArm->bUsePawnControlRotation = true;

    mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    mCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);
    mCamera->bUsePawnControlRotation = false;
}

void ACOAAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("Turn", this, &ACOAAvatar::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &ACOAAvatar::LookUp);
    PlayerInputComponent->BindAxis("MoveForward", this, &ACOAAvatar::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ACOAAvatar::MoveRight);
}

void ACOAAvatar::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void ACOAAvatar::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void ACOAAvatar::MoveForward(float Value)
{
    FRotator Rotation = Controller->GetControlRotation();
    FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
    FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}

void ACOAAvatar::MoveRight(float Value)
{
    FRotator Rotation = Controller->GetControlRotation();
    FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
    FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}