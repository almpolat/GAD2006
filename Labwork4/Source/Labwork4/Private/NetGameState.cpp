#include "NetGameState.h"
#include "NetPlayerState.h"
#include "Net/UnrealNetwork.h"

void ANetGameState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANetGameState, WinningPlayer);
    DOREPLIFETIME(ANetGameState, MatchTimeRemaining);
    DOREPLIFETIME(ANetGameState, bMatchTimerActive);
}

// ── FIX 4 ──────────────────────────────────────────────────────────────────
// Önceki kod: yalnızca WinningPlayer >= 0 kontrol ediliyordu.
// Sorun: WinningPlayer == -2 (mavi takım timer kazanması) durumunda client'larda
// OnVictory() hiç tetiklenmiyordu. Sunucu OnVictory() direkt çağırıyor ama
// client'lar bu RepNotify üzerinden tetikleniyor.
// Düzeltme: WinningPlayer == -2 durumu da eklendi.
//   -1 = henüz kazanan yok (başlangıç değeri, oyun devam ediyor)
//   >= 0 = kırmızı takım kazandı, PlayerIndex değeri
//   -2 = mavi takım timer ile kazandı (Assignment extension)
// ───────────────────────────────────────────────────────────────────────────
void ANetGameState::OnRep_Winner()
{
    if (WinningPlayer >= 0 || WinningPlayer == -2)
    {
        OnVictory();
    }
}

void ANetGameState::TriggerRestart_Implementation()
{
    OnRestart();
}

ANetPlayerState* ANetGameState::GetPlayerStateByIndex(int PlayerIndex)
{
    for (APlayerState* PS : PlayerArray)
    {
        ANetPlayerState* State = Cast<ANetPlayerState>(PS);
        if (State && State->PlayerIndex == PlayerIndex)
            return State;
    }
    return nullptr;
}