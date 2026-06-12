#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NetGameInstance.h"
#include "NetGameState.generated.h"

class ANetPlayerState;

UCLASS()
class LABWORK4_API ANetGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    ANetGameState() : WinningPlayer(-1), MatchTimeRemaining(30.0f), bMatchTimerActive(false) {}

    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Winner)
    int WinningPlayer;

    UFUNCTION()
    void OnRep_Winner();

    UFUNCTION(BlueprintImplementableEvent)
    void OnVictory();

    UFUNCTION(BlueprintImplementableEvent)
    void OnRestart();

    UFUNCTION(NetMulticast, Reliable)
    void TriggerRestart();

    UFUNCTION(BlueprintCallable)
    ANetPlayerState* GetPlayerStateByIndex(int PlayerIndex);

    // Assignment: 30 second timer visible to all players
    UPROPERTY(Replicated, BlueprintReadOnly)
    float MatchTimeRemaining;

    UPROPERTY(Replicated, BlueprintReadOnly)
    bool bMatchTimerActive;
};