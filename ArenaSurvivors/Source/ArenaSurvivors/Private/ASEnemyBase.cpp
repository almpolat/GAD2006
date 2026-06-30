#include "ASEnemyBase.h"
#include "ASAIController.h"
#include "ASBaseCharacter.h"
#include "ASGameMode.h"
#include "ASEnemyHealthBarWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AASEnemyBase::AASEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;

    AttackDamage = 10.f;
    AttackCooldown = 1.5f;
    ScoreValue = 100;
    bCanAttack = true;
    bIsMeleeEnemy = false;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AASAIController::StaticClass();

    GetCharacterMovement()->MaxWalkSpeed = 200.f;

    HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
    HealthBarWidget->SetupAttachment(RootComponent);
    HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
    HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
    HealthBarWidget->SetDrawSize(FVector2D(80.f, 10.f));
}

void AASEnemyBase::BeginPlay()
{
    Super::BeginPlay();
}

void AASEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (HealthBarWidget)
    {
        UASEnemyHealthBarWidget* Widget = Cast<UASEnemyHealthBarWidget>(HealthBarWidget->GetUserWidgetObject());
        if (Widget)
        {
            Widget->SetHealthPercent(GetHealthPercent());
        }
    }
}

void AASEnemyBase::PerformAttack()
{
    if (!bCanAttack || bIsDead) return;

    bCanAttack = false;

    AASAIController* AIC = Cast<AASAIController>(GetController());
    if (AIC && AIC->CurrentTarget)
    {
        AASBaseCharacter* Target = Cast<AASBaseCharacter>(AIC->CurrentTarget);
        if (Target && !Target->bIsDead)
        {
            float MyRadius = GetCapsuleComponent() ? GetCapsuleComponent()->GetScaledCapsuleRadius() : 0.f;
            float TargetRadius = Target->GetCapsuleComponent() ? Target->GetCapsuleComponent()->GetScaledCapsuleRadius() : 0.f;

            float PivotDistance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
            float SurfaceDistance = PivotDistance - MyRadius - TargetRadius;

            if (SurfaceDistance <= 40.f)
            {
                Target->TakeDamage_AS(AttackDamage);
            }
        }
    }

    GetWorldTimerManager().SetTimer(
        AttackCooldownTimer,
        this,
        &AASEnemyBase::ResetAttack,
        AttackCooldown,
        false
    );
}

void AASEnemyBase::ResetAttack()
{
    bCanAttack = true;
}

void AASEnemyBase::Die()
{
    Super::Die();

    if (HasAuthority())
    {
        AASGameMode* GM = Cast<AASGameMode>(GetWorld()->GetAuthGameMode());
        if (GM)
        {
            GM->OnEnemyKilled(nullptr, bIsMeleeEnemy);
        }
    }

    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);

    FTimerHandle DestroyTimer;
    GetWorldTimerManager().SetTimer(DestroyTimer, [this]()
        {
            Destroy();
        }, 2.f, false);
}