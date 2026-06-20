#include "ASRangedEnemy.h"
#include "ASAIController.h"
#include "ASBaseCharacter.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AASRangedEnemy::AASRangedEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    RangedAttackRange = 600.f;
    MinDistance = 200.f;
    AttackDamage = 10.f;
    AttackCooldown = 2.f;
    MaxHealth = 50.f;
}

void AASRangedEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsDead) return;

    AASAIController* AIC = Cast<AASAIController>(GetController());
    if (!AIC || !AIC->CurrentTarget) return;

    float DistToTarget = FVector::Dist(
        GetActorLocation(),
        AIC->CurrentTarget->GetActorLocation()
    );

    if (DistToTarget <= RangedAttackRange && DistToTarget >= MinDistance)
    {
        PerformAttack();
    }
}

void AASRangedEnemy::PerformAttack()
{
    if (!bCanAttack || bIsDead) return;

    AASAIController* AIC = Cast<AASAIController>(GetController());
    if (!AIC || !AIC->CurrentTarget) return;

    // Þimdilik direkt hasar ver (projectile ilerleyen fazda)
    AASBaseCharacter* Target = Cast<AASBaseCharacter>(AIC->CurrentTarget);
    if (Target)
    {
        Target->TakeDamage_AS(AttackDamage);

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
                TEXT("Ranged Enemy attacked!"));
        }
    }

    bCanAttack = false;
    GetWorldTimerManager().SetTimer(
        AttackCooldownTimer,
        this,
        &AASEnemyBase::ResetAttack,
        AttackCooldown,
        false
    );
}