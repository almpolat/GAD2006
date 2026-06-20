#include "ASMeleeEnemy.h"
#include "ASAIController.h"
#include "TimerManager.h"

AASMeleeEnemy::AASMeleeEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    MeleeAttackRange = 150.f;
    AttackDamage = 15.f;
    MaxHealth = 80.f;
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