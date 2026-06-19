#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ASGameState.generated.h"

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
    WaitingToStart  UMETA(DisplayName = "Waiting To Start"),
    InWave          UMETA(DisplayName = "In Wave"),
    BetweenWaves    UMETA(DisplayName = "Between Waves"),
    Victory         UMETA(DisplayName = "Victory"),
    Defeat          UMETA(DisplayName = "Defeat")
};

UCLASS()
class ARENASURVIVORS_API AASGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AASGameState();

    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentWave, BlueprintReadOnly, Category = "Game")
    int32 CurrentWave;

    UPROPERTY(ReplicatedUsing = OnRep_GamePhase, BlueprintReadOnly, Category = "Game")
    EGamePhase GamePhase;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game")
    float WaveTimer;

    UFUNCTION()
    void OnRep_CurrentWave();

    UFUNCTION()
    void OnRep_GamePhase();
};