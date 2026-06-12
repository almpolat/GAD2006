#include "NetAvatar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

ANetAvatar::ANetAvatar() : MovementScale(1.0f), bRunning(false)
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

void ANetAvatar::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANetAvatar, bRunning);
}

void ANetAvatar::BeginPlay()
{
    Super::BeginPlay();

    Camera->bUsePawnControlRotation = false;
    SpringArm->bUsePawnControlRotation = true;
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ANetAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("Lookup", this, &ACharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("MoveForward", this, &ANetAvatar::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ANetAvatar::MoveRight);

    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ANetAvatar::StartRun);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ANetAvatar::StopRun);
}

void ANetAvatar::OnRep_bRunning()
{
    GetCharacterMovement()->MaxWalkSpeed = bRunning ? 600.0f : 300.0f;
}

void ANetAvatar::StartRun()
{
    bRunning = true;
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void ANetAvatar::StopRun()
{
    bRunning = false;
    GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void ANetAvatar::MoveForward(float Amount)
{
    FRotator Rotation = GetController()->GetControlRotation();
    FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
    FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(ForwardDirection, MovementScale * Amount);
}

void ANetAvatar::MoveRight(float Amount)
{
    FRotator Rotation = GetController()->GetControlRotation();
    FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
    FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(RightDirection, MovementScale * Amount);
}