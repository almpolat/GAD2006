#include "NetGameState.h"
#include "NetPlayerState.h"
#include "NetBaseCharacter.h"
#include "Net/UnrealNetwork.h"

ANetGameState::ANetGameState()
	: WinningPlayer(-1)
{
}

void ANetGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetGameState, WinningPlayer);
	DOREPLIFETIME(ANetGameState, MatchTimeRemaining);
	DOREPLIFETIME(ANetGameState, bMatchTimerActive);
}

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
		{
			return State;
		}
	}
	return nullptr;
}

void ANetGameState::StartMatchTimer()
{
	if (HasAuthority())
	{
		MatchTimeRemaining = 30.0f;
		bMatchTimerActive = true;
		GetWorldTimerManager().SetTimer(
			MatchTimerHandle,
			this,
			&ANetGameState::OnMatchTimerTick,
			1.0f,
			true
		);
	}
}

void ANetGameState::OnMatchTimerTick()
{
	if (HasAuthority())
	{
		MatchTimeRemaining -= 1.0f;
		if (MatchTimeRemaining <= 0.0f)
		{
			bMatchTimerActive = false;
			GetWorldTimerManager().ClearTimer(MatchTimerHandle);
			// Blue team survived 30 seconds - blue team wins
			WinningPlayer = -2; // -2 means blue team wins
			OnRep_Winner();
		}
	}
}