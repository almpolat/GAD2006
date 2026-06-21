#include "ASGameMode.h"
#include "ASGameState.h"
#include "ASPlayerController.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

AASGameMode::AASGameMode()
{
    CurrentWave = 0;
    AliveEnemyCount = 0;
    DeadPlayerCount = 0;
    BaseEnemyCount = 3;
    EnemyCountIncrement = 2;
    MaxWaves = 10;
}

void AASGameMode::BeginPlay()
{
    Super::BeginPlay();

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

    // 10 dalga tamamlandı → Victory
    if (CurrentWave > MaxWaves)
    {
        EndGame(true);
        return;
    }

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
            FString::Printf(TEXT("Wave %d / %d started! Enemies: %d"),
                CurrentWave, MaxWaves, AliveEnemyCount));
    }
}

void AASGameMode::SpawnEnemiesForWave()
{
    if (!MeleeEnemyClass) return;

    UWorld* World = GetWorld();
    if (!World) return;

    for (int32 i = 0; i < AliveEnemyCount; i++)
    {
        FVector SpawnLocation = FVector(
            FMath::RandRange(-800.f, 800.f),
            FMath::RandRange(-800.f, 800.f),
            100.f
        );

        FRotator SpawnRotation = FRotator::ZeroRotator;

        // Dalgaya göre ranged düşman ekle
        if (i % 3 == 2 && RangedEnemyClass)
        {
            World->SpawnActor<AActor>(RangedEnemyClass, SpawnLocation, SpawnRotation);
        }
        else
        {
            World->SpawnActor<AActor>(MeleeEnemyClass, SpawnLocation, SpawnRotation);
        }
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

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
                TEXT("Wave cleared! Next wave in 5 seconds..."));
        }

        GetWorldTimerManager().SetTimer(
            WaveStartTimerHandle,
            this,
            &AASGameMode::StartNextWave,
            5.f,
            false
        );
    }
}

void AASGameMode::OnPlayerDied()
{
    DeadPlayerCount++;

    // Kaç oyuncu var
    int32 TotalPlayers = GetNumPlayers();

    if (DeadPlayerCount >= TotalPlayers)
    {
        EndGame(false);
    }
}

void AASGameMode::EndGame(bool bPlayersWon)
{
    AASGameState* GS = GetGameState<AASGameState>();
    if (GS)
    {
        GS->GamePhase = bPlayersWon ? EGamePhase::Victory : EGamePhase::Defeat;
    }

    // Tüm PlayerController'lara EndScreen göster
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AASPlayerController* PC = Cast<AASPlayerController>(It->Get());
        if (PC)
        {
            PC->ShowEndScreen(bPlayersWon);
        }
    }
}