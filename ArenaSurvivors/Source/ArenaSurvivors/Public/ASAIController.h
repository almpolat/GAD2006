#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ASAIController.generated.h"

UCLASS()
class ARENASURVIVORS_API AASAIController : public AAIController
{
    GENERATED_BODY()

public:
    AASAIController();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void OnPossess(APawn* InPawn) override;

    // Hedef oyuncuyu bul
    UFUNCTION(BlueprintCallable, Category = "AI")
    AActor* FindNearestPlayer();

    // Hedefe doðru hareket et
    UFUNCTION(BlueprintCallable, Category = "AI")
    void MoveToTarget(AActor* Target);

    // Saldýrý menzili
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float AttackRange;

    // Takip menzili
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float ChaseRange;

    UPROPERTY(BlueprintReadOnly, Category = "AI")
    AActor* CurrentTarget;

protected:
    FTimerHandle AITickTimer;

    void UpdateAI();
};