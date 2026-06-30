#include "ASPlayerState.h"
#include "ASAvatar.h"
#include "Net/UnrealNetwork.h"

AASPlayerState::AASPlayerState()
{
    PlayerScore = 0;
    KillCount = 0;
    PlayerNickname = TEXT("Player");
    CostumeIndex = 0;
    CostumeRowName = FName("Costume1");
}

void AASPlayerState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AASPlayerState, PlayerScore);
    DOREPLIFETIME(AASPlayerState, KillCount);
    DOREPLIFETIME(AASPlayerState, PlayerNickname);
    DOREPLIFETIME(AASPlayerState, CostumeIndex);
    DOREPLIFETIME(AASPlayerState, CostumeRowName);
}

void AASPlayerState::OnRep_CostumeRowName()
{
    // Bu PlayerState'in sahibi olan Pawn'ý bul ve costume uygula
    APawn* OwnerPawn = GetPawn();
    if (!OwnerPawn) return;

    AASAvatar* Avatar = Cast<AASAvatar>(OwnerPawn);
    if (Avatar)
    {
        Avatar->ApplyCostumeFromPlayerState();
    }
}

void AASPlayerState::AddScore(int32 Amount)
{
    if (HasAuthority())
        PlayerScore += Amount;
}

void AASPlayerState::AddKill()
{
    if (HasAuthority())
    {
        KillCount++;
        AddScore(100);
    }
}