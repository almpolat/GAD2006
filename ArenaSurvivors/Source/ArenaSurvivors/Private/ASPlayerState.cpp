#include "ASPlayerState.h"
#include "Net/UnrealNetwork.h"

AASPlayerState::AASPlayerState()
{
    PlayerScore = 0;
    KillCount = 0;
    PlayerNickname = TEXT("Player");
    CostumeIndex = 0;
}

void AASPlayerState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AASPlayerState, PlayerScore);
    DOREPLIFETIME(AASPlayerState, KillCount);
    DOREPLIFETIME(AASPlayerState, PlayerNickname);
    DOREPLIFETIME(AASPlayerState, CostumeIndex);
}

void AASPlayerState::AddScore(int32 Amount)
{
    if (HasAuthority())
    {
        PlayerScore += Amount;
    }
}

void AASPlayerState::AddKill()
{
    if (HasAuthority())
    {
        KillCount++;
        AddScore(100);
    }
}