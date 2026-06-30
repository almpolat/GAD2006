#include "ASAvatar.h"
#include "ASGameInstance.h"
#include "ASPlayerState.h"
#include "ASPlayerNameTagWidget.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "WorldCollision.h"
#include "Engine/OverlapResult.h"

AASAvatar::AASAvatar()
{
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
    GetCharacterMovement()->SetIsReplicated(true);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 600.f;
    SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    SpringArm->bUsePawnControlRotation = false;
    SpringArm->bInheritPitch = false;
    SpringArm->bInheritYaw = false;
    SpringArm->bInheritRoll = false;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    Camera->bUsePawnControlRotation = false;

    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;

    DashDistance = 1200.f;
    DashCooldown = 1.5f;
    bCanDash = true;

    MeleeDamage = 25.f;
    MeleeRange = 155.f;
    bIsAttacking = false;

    ReplicatedCostumeRowName = FName("Costume1");

    NameTagWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameTagWidget"));
    NameTagWidget->SetupAttachment(RootComponent);
    NameTagWidget->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
    NameTagWidget->SetWidgetSpace(EWidgetSpace::Screen);
    NameTagWidget->SetDrawSize(FVector2D(120.f, 20.f));
}

void AASAvatar::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AASAvatar, ReplicatedCostumeRowName);
}

void AASAvatar::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocallyControlled())
    {
        UASGameInstance* GI = Cast<UASGameInstance>(GetGameInstance());
        if (GI)
        {
            Server_SetPlayerInfo(GI->PlayerNickname, GI->CostumeIndex, GI->CostumeRowName);
        }
    }
}

void AASAvatar::OnRep_CostumeRowName()
{
    ApplyCostumeFromPlayerState();
}

void AASAvatar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (NameTagWidget)
    {
        UASPlayerNameTagWidget* Widget = Cast<UASPlayerNameTagWidget>(NameTagWidget->GetUserWidgetObject());
        if (Widget)
        {
            AASPlayerState* PS = GetPlayerState<AASPlayerState>();
            if (PS)
            {
                Widget->SetNickname(PS->PlayerNickname);
            }
        }
    }
}

void AASAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (!IsLocallyControlled()) return;

    PlayerInputComponent->BindAxis("MoveForward", this, &AASAvatar::MoveForward);
    PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AASAvatar::Dash);
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AASAvatar::MeleeAttack);
}

void AASAvatar::MoveForward(float Value)
{
    if (!IsLocallyControlled()) return;
    if (bIsDead || FMath::IsNearlyZero(Value)) return;

    FVector Direction = GetActorForwardVector();
    Direction.Z = 0.f;
    Direction.Normalize();
    AddMovementInput(Direction, Value);
}

void AASAvatar::Dash()
{
    if (!IsLocallyControlled()) return;
    if (!bCanDash || bIsDead) return;

    FVector DashDirection = GetActorForwardVector();
    DashDirection.Z = 0.f;
    DashDirection.Normalize();

    Server_Dash(DashDirection);
}

void AASAvatar::Server_Dash_Implementation(FVector Direction)
{
    bCanDash = false;
    LaunchCharacter(Direction * DashDistance, true, true);
    GetWorldTimerManager().SetTimer(
        DashCooldownTimer, this, &AASAvatar::ResetDash, DashCooldown, false);
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
    if (!IsLocallyControlled()) return;
    if (bIsAttacking || bIsDead) return;

    FVector CapsuleCenter = GetActorLocation() + GetActorForwardVector() * 120.f;
    CapsuleCenter.Z = GetActorLocation().Z;

    FRotator CapsuleRotation = GetActorRotation();
    FQuat CapsuleQuat = (CapsuleRotation + FRotator(90.f, 0.f, 0.f)).Quaternion();

    DrawDebugCapsule(
        GetWorld(),
        CapsuleCenter,
        140.f,
        30.f,
        CapsuleQuat,
        FColor::Red,
        false,
        0.5f
    );

    Server_MeleeAttack();
}

void AASAvatar::Server_MeleeAttack_Implementation()
{
    bIsAttacking = true;
    PerformMeleeTrace();

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
    FVector CapsuleCenter = GetActorLocation() + GetActorForwardVector() * 120.f;
    CapsuleCenter.Z = GetActorLocation().Z;

    FQuat CapsuleQuat = (GetActorRotation() + FRotator(90.f, 0.f, 0.f)).Quaternion();

    TArray<FOverlapResult> Overlaps;

    FCollisionObjectQueryParams ObjectParams;
    ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
    ObjectParams.AddObjectTypesToQuery(ECC_PhysicsBody);
    ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.bTraceComplex = false;

    bool bHit = GetWorld()->OverlapMultiByObjectType(
        Overlaps,
        CapsuleCenter,
        CapsuleQuat,
        ObjectParams,
        FCollisionShape::MakeCapsule(30.f, 140.f),
        QueryParams
    );

    if (Overlaps.Num() == 0) return;

    FVector MyForward = GetActorForwardVector();
    TSet<AActor*> AlreadyDamaged;

    for (const FOverlapResult& Result : Overlaps)
    {
        AActor* HitActor = Result.GetActor();
        if (!HitActor && Result.GetComponent())
        {
            HitActor = Result.GetComponent()->GetOwner();
        }
        if (!HitActor || HitActor == this) continue;
        if (AlreadyDamaged.Contains(HitActor)) continue;

        AASBaseCharacter* Target = Cast<AASBaseCharacter>(HitActor);
        if (!Target || Target->bIsDead) continue;

        FVector ToTarget = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        float Dot = FVector::DotProduct(MyForward, ToTarget);

        if (Dot > -0.3f)
        {
            Target->TakeDamage_AS(MeleeDamage);
            AlreadyDamaged.Add(HitActor);
        }
    }
}

void AASAvatar::Server_SetRotation_Implementation(FRotator NewRotation)
{
    SetActorRotation(NewRotation);
}

void AASAvatar::Server_SetPlayerInfo_Implementation(
    const FString& Nickname, int32 InCostumeIndex, FName InCostumeRowName)
{
    AASPlayerState* PS = GetPlayerState<AASPlayerState>();
    if (PS)
    {
        PS->PlayerNickname = Nickname;
        PS->CostumeIndex = InCostumeIndex;
        PS->CostumeRowName = InCostumeRowName;
    }

    ReplicatedCostumeRowName = InCostumeRowName;
}