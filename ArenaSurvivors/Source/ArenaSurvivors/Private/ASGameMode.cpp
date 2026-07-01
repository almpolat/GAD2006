#include "ASGameMode.h"
#include "ASGameState.h"
#include "ASPlayerController.h"
#include "ASPlayerState.h"
#include "ASGameInstance.h"
#include "ASBaseCharacter.h"
#include "ASHealthPickup.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "NavigationSystem.h"

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

    HealthPickupCount = 2;
    HealthPickupSpawnRadius = 1500.f;
}

void AASGameMode::BeginPlay()
{
    Super::BeginPlay();
    AlivePlayerCount = 0;
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
    ClearHealthPickups();
    SpawnHealthPickups();
}

void AASGameMode::SpawnEnemiesForWave()
{
    if (!MeleeEnemyClass) return;

    UWorld* World = GetWorld();
    if (!World) return;

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);

    for (int32 i = 0; i < AliveEnemyCount; i++)
    {
        FVector SpawnLocation = FVector(0.f, 0.f, 100.f);

        if (NavSys)
        {
            FNavLocation ResultLocation;
            bool bFound = NavSys->GetRandomReachablePointInRadius(
                FVector::ZeroVector, 800.f, ResultLocation);

            if (bFound)
            {
                SpawnLocation = ResultLocation.Location + FVector(0.f, 0.f, 50.f);
            }
        }

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

void AASGameMode::ClearHealthPickups()
{
    for (AActor* Pickup : ActiveHealthPickups)
    {
        if (IsValid(Pickup))
        {
            Pickup->Destroy();
        }
    }
    ActiveHealthPickups.Empty();
}

void AASGameMode::SpawnHealthPickups()
{
    if (!HealthPickupClass) return;
    if (!HasAuthority()) return;

    UWorld* World = GetWorld();
    if (!World) return;

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
    if (!NavSys) return;

    FVector Origin = FVector::ZeroVector;

    for (int32 i = 0; i < HealthPickupCount; i++)
    {
        FNavLocation ResultLocation;
        bool bFound = NavSys->GetRandomReachablePointInRadius(Origin, HealthPickupSpawnRadius, ResultLocation);

        if (bFound)
        {
            FVector SpawnLoc = ResultLocation.Location + FVector(0.f, 0.f, 30.f);
            FActorSpawnParameters Params;
            Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            AActor* NewPickup = World->SpawnActor<AActor>(HealthPickupClass, SpawnLoc, FRotator::ZeroRotator, Params);
            if (NewPickup)
            {
                ActiveHealthPickups.Add(NewPickup);
            }
        }
    }
}

void AASGameMode::OnEnemyKilled(APlayerController* Killer, bool bWasMeleeEnemy)
{
    if (bGameEnded) return;

    AliveEnemyCount--;

    AASGameState* GS = GetGameState<AASGameState>();
    if (GS)
    {
        GS->TotalKills++;
        if (bWasMeleeEnemy)
            GS->MeleeEnemyKills++;
        else
            GS->RangedEnemyKills++;
    }

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

        float NextWaveDelay = (CurrentWave >= MaxWaves) ? 1.5f : 5.f;

        GetWorldTimerManager().SetTimer(
            WaveStartTimerHandle,
            this,
            &AASGameMode::StartNextWave,
            NextWaveDelay,
            false
        );
    }
}

void AASGameMode::OnPlayerDied(APlayerController* DeadPlayerController)
{
    if (bGameEnded) return;

    AActor* AliveTarget = nullptr;

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (!PC || PC == DeadPlayerController) continue;

        APawn* OtherPawn = PC->GetPawn();
        if (!OtherPawn) continue;

        AASBaseCharacter* OtherChar = Cast<AASBaseCharacter>(OtherPawn);
        if (OtherChar && !OtherChar->bIsDead)
        {
            AliveTarget = OtherPawn;
            break;
        }
    }

    if (AliveTarget)
    {
        AASPlayerController* DeadPC = Cast<AASPlayerController>(DeadPlayerController);
        if (DeadPC)
        {
            DeadPC->ClientSpectatePlayer(AliveTarget);
        }
    }
    else
    {
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
        if (!PC) continue;

        UASGameInstance* GI = Cast<UASGameInstance>(PC->GetGameInstance());
        if (GI && GS)
        {
            GI->bLastGameVictory = bPlayersWon;
            GI->LastWaveReached = GS->CurrentWave;
            GI->LastTotalKills = GS->TotalKills;
            GI->LastMeleeKills = GS->MeleeEnemyKills;
            GI->LastRangedKills = GS->RangedEnemyKills;
        }

        if (GS)
        {
            PC->Client_StoreEndGameResults(
                bPlayersWon,
                GS->CurrentWave,
                GS->TotalKills,
                GS->MeleeEnemyKills,
                GS->RangedEnemyKills
            );
        }
    }

    FTimerHandle EndTravelTimer;
    GetWorldTimerManager().SetTimer(EndTravelTimer, [this]()
        {
            GetWorld()->ServerTravel(TEXT("/Game/Maps/EndScreenMap?listen"));
        }, 1.f, false);
}