#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ASGameMode.generated.h"

UCLASS()
class ARENASURVIVORS_API AASGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AASGameMode();

    virtual void BeginPlay() override;

    // Dalga sistemi
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void StartNextWave();

    UFUNCTION(BlueprintCallable, Category = "Wave")
    void OnEnemyKilled();

    UFUNCTION(BlueprintCallable, Category = "Wave")
    void EndGame(bool bPlayersWon);

    // Spawn edilecek düþman sýnýflarý (Blueprint'te atanacak)
    UPROPERTY(EditDefaultsOnly, Category = "Wave")
    TSubclassOf<AActor> MeleeEnemyClass;

    UPROPERTY(EditDefaultsOnly, Category = "Wave")
    TSubclassOf<AActor> RangedEnemyClass;

    // Her dalgada kaç düþman spawn olur
    UPROPERTY(EditDefaultsOnly, Category = "Wave")
    int32 BaseEnemyCount;

    // Her dalgada kaç düþman eklenir
    UPROPERTY(EditDefaultsOnly, Category = "Wave")
    int32 EnemyCountIncrement;

protected:
    int32 CurrentWave;
    int32 AliveEnemyCount;

    void SpawnEnemiesForWave();

    FTimerHandle WaveStartTimerHandle;
};