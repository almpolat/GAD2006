#include "ASGameMode.h"
#include "ASGameState.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "NavigationSystem.h"
#include "TimerManager.h"

AASGameMode::AASGameMode()
{
    CurrentWave = 0;
    AliveEnemyCount = 0;
    BaseEnemyCount = 3;
    EnemyCountIncrement = 2;
}

void AASGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Ýlk dalgayý 3 saniye sonra baþlat
    GetWorldTimerManager().SetTimer(
        WaveStartTimerHandle,
        this,
        &AASGameMode::StartNextWave,
        3.f,
        false
    );
}

void AASGameMode::StartNextWave()
{
    CurrentWave++;
    AliveEnemyCount = BaseEnemyCount + (EnemyCountIncrement * (CurrentWave - 1));

    AASGameState* GS = GetGameState<AASGameState>();
    if (GS)
    {
        GS->CurrentWave = CurrentWave;
        GS->GamePhase = EGamePhase::InWave;
    }

    SpawnEnemiesForWave();

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange,
            FString::Printf(TEXT("Wave %d started! Enemies: %d"), CurrentWave, AliveEnemyCount));
    }
}

void AASGameMode::SpawnEnemiesForWave()
{
    if (!MeleeEnemyClass) return;

    UWorld* World = GetWorld();
    if (!World) return;

    for (int32 i = 0; i < AliveEnemyCount; i++)
    {
        // Spawn konumu ilerleyen fazda spawn point'lerden gelecek
        FVector SpawnLocation = FVector(
            FMath::RandRange(-500.f, 500.f),
            FMath::RandRange(-500.f, 500.f),
            100.f
        );

        FRotator SpawnRotation = FRotator::ZeroRotator;
        World->SpawnActor<AActor>(MeleeEnemyClass, SpawnLocation, SpawnRotation);
    }
}

void AASGameMode::OnEnemyKilled()
{
    AliveEnemyCount--;

    if (AliveEnemyCount <= 0)
    {
        AASGameState* GS = GetGameState<AASGameState>();
        if (GS)
        {
            GS->GamePhase = EGamePhase::BetweenWaves;
        }

        // 5 saniye sonra yeni dalga
        GetWorldTimerManager().SetTimer(
            WaveStartTimerHandle,
            this,
            &AASGameMode::StartNextWave,
            5.f,
            false
        );
    }
}

void AASGameMode::EndGame(bool bPlayersWon)
{
    AASGameState* GS = GetGameState<AASGameState>();
    if (GS)
    {
        GS->GamePhase = bPlayersWon ? EGamePhase::Victory : EGamePhase::Defeat;
    }

    if (GEngine)
    {
        FString Msg = bPlayersWon ? TEXT("Players Win!") : TEXT("Game Over!");
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, Msg);
    }
}