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
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float AttackDamage;

    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float AttackCooldown;

    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    int32 ScoreValue;

    UPROPERTY(VisibleAnywhere, Category = "Combat")
    bool bIsMeleeEnemy;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    class UWidgetComponent* HealthBarWidget;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual void PerformAttack();

    virtual void Die() override;

protected:
    bool bCanAttack;
    FTimerHandle AttackCooldownTimer;

public:
    void ResetAttack();
};