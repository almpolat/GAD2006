#include "ASGameState.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

AASGameState::AASGameState()
{
    CurrentWave = 0;
    GamePhase = EGamePhase::WaitingToStart;
    WaveTimer = 0.f;
}

void AASGameState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AASGameState, CurrentWave);
    DOREPLIFETIME(AASGameState, GamePhase);
    DOREPLIFETIME(AASGameState, WaveTimer);
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
    // HUD güncellemesi ilerleyen fazda PlayerController üzerinden yapýlacak
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
            TEXT("Game phase changed."));
    }
}