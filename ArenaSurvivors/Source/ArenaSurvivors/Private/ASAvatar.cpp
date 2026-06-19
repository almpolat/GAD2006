#include "ASAvatar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

AASAvatar::AASAvatar()
{
    PrimaryActorTick.bCanEverTick = true;

    // Spring Arm
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 600.f;
    SpringArm->SetRelativeRotation(FRotator(-50.f, 0.f, 0.f));
    SpringArm->bUsePawnControlRotation = false;
    SpringArm->bInheritPitch = false;
    SpringArm->bInheritYaw = false;
    SpringArm->bInheritRoll = false;

    // Camera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    Camera->bUsePawnControlRotation = false;

    // Hareket
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
    GetCharacterMovement()->MaxWalkSpeed = 500.f;

    // Dash
    DashDistance = 1200.f;
    DashCooldown = 1.5f;
    bCanDash = true;

    // Melee
    MeleeDamage = 25.f;
    MeleeRange = 120.f;
    bIsAttacking = false;
}

void AASAvatar::BeginPlay()
{
    Super::BeginPlay();
}

void AASAvatar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AASAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AASAvatar::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AASAvatar::MoveRight);
    PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AASAvatar::Dash);
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AASAvatar::MeleeAttack);
}

void AASAvatar::MoveForward(float Value)
{
    if (bIsDead || FMath::IsNearlyZero(Value)) return;

    const FVector Direction = FVector(1.f, 0.f, 0.f);
    AddMovementInput(Direction, Value);
}

void AASAvatar::MoveRight(float Value)
{
    if (bIsDead || FMath::IsNearlyZero(Value)) return;

    const FVector Direction = FVector(0.f, 1.f, 0.f);
    AddMovementInput(Direction, Value);
}

void AASAvatar::Dash()
{
    if (!bCanDash || bIsDead) return;

    FVector DashDirection = GetVelocity().GetSafeNormal();
    if (DashDirection.IsNearlyZero())
    {
        DashDirection = GetActorForwardVector();
    }

    Server_Dash(DashDirection);
}

void AASAvatar::Server_Dash_Implementation(FVector Direction)
{
    bCanDash = false;

    LaunchCharacter(Direction * DashDistance, true, true);

    GetWorldTimerManager().SetTimer(
        DashCooldownTimer,
        this,
        &AASAvatar::ResetDash,
        DashCooldown,
        false
    );
}

bool AASAvatar::Server_Dash_Validate(FVector Direction)
{
    return true;
}

void AASAvatar::ResetDash()
{
    bCanDash = true;
}

void AASAvatar::MeleeAttack()
{
    if (bIsAttacking || bIsDead) return;
    Server_MeleeAttack();
}

void AASAvatar::Server_MeleeAttack_Implementation()
{
    bIsAttacking = true;
    PerformMeleeTrace();

    // Saldýrý animasyonu ilerleyen fazda eklenecek
    // Þimdilik 0.5 saniye sonra sýfýrla
    FTimerHandle AttackTimer;
    GetWorldTimerManager().SetTimer(AttackTimer, [this]()
        {
            bIsAttacking = false;
        }, 0.5f, false);
}

bool AASAvatar::Server_MeleeAttack_Validate()
{
    return true;
}

void AASAvatar::PerformMeleeTrace()
{
    FVector Start = GetActorLocation();
    FVector End = Start + GetActorForwardVector() * MeleeRange;

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        Start,
        End,
        FQuat::Identity,
        ECC_Pawn,
        FCollisionShape::MakeSphere(50.f),
        Params
    );

    if (bHit)
    {
        AASBaseCharacter* Target = Cast<AASBaseCharacter>(HitResult.GetActor());
        if (Target)
        {
            Target->TakeDamage_AS(MeleeDamage);
        }
    }

    // Debug çizgisi (geliþtirme sýrasýnda görünür)
    DrawDebugSphere(GetWorld(), End, 50.f, 8, FColor::Red, false, 0.5f);
}