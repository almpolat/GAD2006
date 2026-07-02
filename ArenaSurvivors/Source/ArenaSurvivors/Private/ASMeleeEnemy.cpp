#include "ASMeleeEnemy.h"
#include "ASAIController.h"
#include "TimerManager.h"
AASMeleeEnemy::AASMeleeEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    MeleeAttackRange = 80.f;  // 150'den 100'e
    AttackDamage = 8.f;        // 15'ten 5'e
    MaxHealth = 80.f;
    bIsMeleeEnemy = true;
}

void AASMeleeEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsDead) return;

    AASAIController* AIC = Cast<AASAIController>(GetController());
    if (!AIC || !AIC->CurrentTarget) return;

    float DistToTarget = FVector::Dist(
        GetActorLocation(),
        AIC->CurrentTarget->GetActorLocation()
    );

    if (DistToTarget <= MeleeAttackRange)
    {
        PerformAttack();
    }
}

void AASMeleeEnemy::PerformAttack()
{
    Super::PerformAttack();
}