#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ASBaseCharacter.generated.h"

UCLASS()
class ARENASURVIVORS_API AASBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AASBaseCharacter();

    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Can sistemi
    UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Stats")
    float Health;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float MaxHealth;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Stats")
    bool bIsDead;

    // Hasar alma
    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual void TakeDamage_AS(float DamageAmount);

    // Ölüm
    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual void Die();

    // Can yüzdesi (HUD için)
    UFUNCTION(BlueprintPure, Category = "Stats")
    float GetHealthPercent() const;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnRep_Health();

    // Blueprint'te override edilebilir ölüm eventi
    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnDeath();
};