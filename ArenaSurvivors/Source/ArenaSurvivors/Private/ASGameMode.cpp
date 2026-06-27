#include "ASGameMode.h"
#include "ASGameState.h"
#include "ASPlayerController.h"
#include "ASPlayerState.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

AASGameMode::AASGameMode()
{
    CurrentWave = 0;
    AliveEnemyCount = 0;
    AlivePlayerCount = 0;
    BaseEnemyCount = 3;
    EnemyCountIncrement = 2;
    MaxWaves = 10;
    bGameStarted = false;
    bGameEnded = false;
}

void AASGameMode::BeginPlay()
{
    Super::BeginPlay();
    AlivePlayerCount = 0;
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
            TEXT("ArenaMap loaded. Waiting for players to exit door..."));
    }
}

void AASGameMode::StartNextWave()
{
    if (bGameEnded) return;

    bGameStarted = true;
    AlivePlayerCount = GetNumPlayers();
    CurrentWave++;

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

void AASGameMode::OnEnemyKilled(APlayerController* Killer, bool bWasMeleeEnemy)
{
    if (bGameEnded) return;

    AliveEnemyCount--;

    // Kill istatistiklerini güncelle
    AASGameState* GS = GetGameState<AASGameState>();
    if (GS)
    {
        GS->TotalKills++;
        if (bWasMeleeEnemy)
            GS->MeleeEnemyKills++;
        else
            GS->RangedEnemyKills++;
    }

    // Killer PlayerState'e kill ekle
    if (Killer)
    {
        AASPlayerState* PS = Killer->GetPlayerState<AASPlayerState>();
        if (PS)
        {
            PS->AddKill();
        }
    }

    if (AliveEnemyCount <= 0)
    {
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

void AASGameMode::OnPlayerDied(APlayerController* DeadPlayerController)
{
    if (bGameEnded) return;

    // Yaşayan oyuncu ara
    AActor* AliveTarget = nullptr;

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();

        // Ölen oyuncuyu atla
        if (!PC || PC == DeadPlayerController) continue;

        // Bu oyuncunun pawn'ı var mı?
        APawn* AlivePawn = PC->GetPawn();
        if (AlivePawn)
        {
            AliveTarget = AlivePawn;
            break;
        }
    }

    if (AliveTarget)
    {
        // Hala yaşayan var → spectator yap
        AASPlayerController* DeadPC = Cast<AASPlayerController>(DeadPlayerController);
        if (DeadPC)
        {
            DeadPC->ClientSpectatePlayer(AliveTarget);
        }

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
                TEXT("One player died. Other player continues!"));
        }
    }
    else
    {
        // Hiç yaşayan yok → Defeat
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
                TEXT("Both players dead! Game Over."));
        }

        EndGame(false);
    }
}

void AASGameMode::EndGame(bool bPlayersWon)
{
    if (bGameEnded) return;
    bGameEnded = true;

    GetWorldTimerManager().ClearTimer(WaveStartTimerHandle);

    AASGameState* GS = GetGameState<AASGameState>();
    if (GS)
    {
        GS->GamePhase = bPlayersWon ? EGamePhase::Victory : EGamePhase::Defeat;
    }

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AASPlayerController* PC = Cast<AASPlayerController>(It->Get());
        if (PC)
        {
            PC->ShowEndScreen(bPlayersWon);
        }
    }
}