#pragma once

#include "CoreMinimal.h"
#include "ASEnemyBase.h"
#include "ASRangedEnemy.generated.h"

UCLASS()
class ARENASURVIVORS_API AASRangedEnemy : public AASEnemyBase
{
    GENERATED_BODY()

public:
    AASRangedEnemy();

    virtual void Tick(float DeltaTime) override;

    virtual void PerformAttack() override;

    // Menzilli saldýrý mesafesi
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float RangedAttackRange;

    // Minimum mesafe (çok yaklaþýrsa geri çekilir)
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float MinDistance;

    // Mermi sýnýfý (ilerleyen fazda)
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    TSubclassOf<AActor> ProjectileClass;
};