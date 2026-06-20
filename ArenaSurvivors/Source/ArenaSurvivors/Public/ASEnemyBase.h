#pragma once

#include "CoreMinimal.h"
#include "ASBaseCharacter.h"
#include "ASEnemyBase.generated.h"

UCLASS()
class ARENASURVIVORS_API AASEnemyBase : public AASBaseCharacter
{
    GENERATED_BODY()

public:
    AASEnemyBase();

    virtual void BeginPlay() override;

    // Saldýrý hasarý
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float AttackDamage;

    // Saldýrý cooldown
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float AttackCooldown;

    // Öldüðünde puan ver
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    int32 ScoreValue;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual void PerformAttack();

    virtual void Die() override;

protected:
    bool bCanAttack;
    FTimerHandle AttackCooldownTimer;

public:
    void ResetAttack();
};