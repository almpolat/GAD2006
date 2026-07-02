#include "ASRangedEnemy.h"
#include "ASAIController.h"
#include "ASBaseCharacter.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AASRangedEnemy::AASRangedEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    RangedAttackRange = 200.f;  // 600'den 400'e
    MinDistance = 100.f;
    AttackDamage = 5.f;         // 10'dan 0'a... 
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

    AASBaseCharacter* Target = Cast<AASBaseCharacter>(AIC->CurrentTarget);
    if (Target)
    {
        Target->TakeDamage_AS(AttackDamage);
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