#pragma once

#include "CoreMinimal.h"
#include "ASEnemyBase.h"
#include "ASMeleeEnemy.generated.h"

UCLASS()
class ARENASURVIVORS_API AASMeleeEnemy : public AASEnemyBase
{
    GENERATED_BODY()

public:
    AASMeleeEnemy();

    virtual void Tick(float DeltaTime) override;

    virtual void PerformAttack() override;

    // Saldýrý menzili kontrolü
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float MeleeAttackRange;
};