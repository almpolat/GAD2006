#include "ASGameState.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

AASGameState::AASGameState()
{
    CurrentWave = 0;
    GamePhase = EGamePhase::WaitingToStart;
    WaveTimer = 0.f;
    TotalKills = 0;
    MeleeEnemyKills = 0;
    RangedEnemyKills = 0;
}

void AASGameState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AASGameState, CurrentWave);
    DOREPLIFETIME(AASGameState, GamePhase);
    DOREPLIFETIME(AASGameState, WaveTimer);
    DOREPLIFETIME(AASGameState, TotalKills);
    DOREPLIFETIME(AASGameState, MeleeEnemyKills);
    DOREPLIFETIME(AASGameState, RangedEnemyKills);
}

void AASGameState::OnRep_CurrentWave()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White,
            FString::Printf(TEXT("Wave: %d"), CurrentWave));
    }
}

void AASGameState::OnRep_GamePhase()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
            TEXT("Game phase changed."));
    }
}