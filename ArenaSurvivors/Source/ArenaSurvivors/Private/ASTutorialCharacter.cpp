#include "ASTutorialCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"

AASTutorialCharacter::AASTutorialCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
    GetCharacterMovement()->MaxWalkSpeed = 400.f;
    GetCharacterMovement()->JumpZVelocity = 0.f;
    GetCharacterMovement()->AirControl = 0.f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 450.f;
    CameraBoom->SocketOffset = FVector(0.f, 0.f, 80.f);
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bDoCollisionTest = false;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

void AASTutorialCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Pitch'i sabit -15 dereceye kilitle
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        PC->SetControlRotation(FRotator(-15.f, GetActorRotation().Yaw, 0.f));

        // Camera manager pitch kilidini sýkýþtýr
        if (PC->PlayerCameraManager)
        {
            PC->PlayerCameraManager->ViewPitchMin = -15.f;
            PC->PlayerCameraManager->ViewPitchMax = -15.f;
        }
    }
}

void AASTutorialCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AASTutorialCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AASTutorialCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &AASTutorialCharacter::TurnRate);
    // LookUp baðlanmýyor — dikey kamera hareketi yok
}

void AASTutorialCharacter::MoveForward(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;
    const FRotator Rot(0.f, GetControlRotation().Yaw, 0.f);
    AddMovementInput(FRotationMatrix(Rot).GetUnitAxis(EAxis::X), Value);
}

void AASTutorialCharacter::MoveRight(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;
    const FRotator Rot(0.f, GetControlRotation().Yaw, 0.f);
    AddMovementInput(FRotationMatrix(Rot).GetUnitAxis(EAxis::Y), Value);
}

void AASTutorialCharacter::TurnRate(float Value)
{
    AddControllerYawInput(Value);
}