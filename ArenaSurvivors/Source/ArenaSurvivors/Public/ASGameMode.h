#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ASGameState.h"
#include "ASGameMode.generated.h"

UCLASS()
class ARENASURVIVORS_API AASGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AASGameMode();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Wave")
    void StartNextWave();

    UFUNCTION(BlueprintCallable, Category = "Wave")
    void OnEnemyKilled(APlayerController* Killer, bool bWasMeleeEnemy);

    UFUNCTION(BlueprintCallable, Category = "Wave")
    void EndGame(bool bPlayersWon);

    UFUNCTION(BlueprintCallable, Category = "Wave")
    void OnPlayerDied(APlayerController* DeadPlayerController);

    UPROPERTY(EditDefaultsOnly, Category = "Wave")
    TSubclassOf<AActor> MeleeEnemyClass;

    UPROPERTY(EditDefaultsOnly, Category = "Wave")
    TSubclassOf<AActor> RangedEnemyClass;

    UPROPERTY(EditDefaultsOnly, Category = "Wave")
    int32 BaseEnemyCount;

    UPROPERTY(EditDefaultsOnly, Category = "Wave")
    int32 EnemyCountIncrement;

    UPROPERTY(EditDefaultsOnly, Category = "Wave")
    int32 MaxWaves;

protected:
    int32 CurrentWave;
    int32 AliveEnemyCount;
    int32 AlivePlayerCount;
    bool bGameStarted;
    bool bGameEnded;

    void SpawnEnemiesForWave();

    FTimerHandle WaveStartTimerHandle;
};