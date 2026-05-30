#include "NetGameMode.h"
#include "NetBaseCharacter.h"
#include "NetGameState.h"
#include "NetPlayerState.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Components/CapsuleComponent.h"

ANetGameMode::ANetGameMode()
{
	DefaultPawnClass = ANetBaseCharacter::StaticClass();
	PlayerStateClass = ANetPlayerState::StaticClass();
	GameStateClass = ANetGameState::StaticClass();

	TotalPlayerCount = 0;
	TotalGames = 0;
	PlayerStartIndex = 0;
}

AActor* ANetGameMode::GetPlayerStart(FString Name, int Index)
{
	FName PSName;
	if (Index < 0)
	{
		PSName = *Name;
	}
	else
	{
		PSName = *FString::Printf(TEXT("%s%d"), *Name, Index % 4);
	}

	for (TActorIterator<APlayerStart> It(GWorld); It; ++It)
	{
		if (APlayerStart* PS = Cast<APlayerStart>(*It))
		{
			if (PS->PlayerStartTag == PSName) return *It;
		}
	}
	return nullptr;
}

AActor* ANetGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	AActor* Start = AssignTeamAndPlayerStart(Player);
	return Start ? Start : Super::ChoosePlayerStart_Implementation(Player);
}

AActor* ANetGameMode::AssignTeamAndPlayerStart(AController* Player)
{
	AActor* Start = nullptr;
	ANetPlayerState* State = Player->GetPlayerState<ANetPlayerState>();

	if (State)
	{
		if (TotalGames == 0)
		{
			State->TeamID = TotalPlayerCount == 0 ?
				EPlayerTeam::TEAM_Blue : EPlayerTeam::TEAM_Red;
			State->PlayerIndex = TotalPlayerCount++;
			AllPlayers.Add(Cast<APlayerController>(Player));
		}
		else
		{
			State->TeamID = State->Result == EGameResults::RESULT_Won ?
				EPlayerTeam::TEAM_Blue : EPlayerTeam::TEAM_Red;
		}

		if (State->TeamID == EPlayerTeam::TEAM_Blue)
		{
			Start = GetPlayerStart("Blue", -1);
		}
		else
		{
			Start = GetPlayerStart("Red", PlayerStartIndex++);
		}
	}

	return Start;
}

void ANetGameMode::AvatarsOverlapped(ANetAvatar* AvatarA, ANetAvatar* AvatarB)
{
	ANetGameState* GState = GetGameState<ANetGameState>();
	if (GState == nullptr || GState->WinningPlayer >= 0) return;

	ANetPlayerState* StateA = AvatarA->GetPlayerState<ANetPlayerState>();
	ANetPlayerState* StateB = AvatarB->GetPlayerState<ANetPlayerState>();

	if (StateA->TeamID == StateB->TeamID) return;

	if (StateA->TeamID == EPlayerTeam::TEAM_Red)
	{
		GState->WinningPlayer = StateA->PlayerIndex;
	}
	else
	{
		GState->WinningPlayer = StateB->PlayerIndex;
	}

	AvatarA->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	AvatarB->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	GState->OnVictory();

	for (APlayerController* Player : AllPlayers)
	{
		auto State = Player->GetPlayerState<ANetPlayerState>();
		if (State->TeamID == EPlayerTeam::TEAM_Blue)
		{
			State->Result = EGameResults::RESULT_Lost;
		}
		else
		{
			State->Result = EGameResults::RESULT_Won;
		}
	}

	FTimerHandle EndGameTimerHandle;
	GWorld->GetTimerManager().SetTimer(EndGameTimerHandle, this,
		&ANetGameMode::EndGame, 2.5f, false);
}

void ANetGameMode::EndGame()
{
	PlayerStartIndex = 0;
	TotalGames++;

	ANetGameState* GState = GetGameState<ANetGameState>();

	// Team switch logic:
	// If blue team won (survived 30 seconds), we switch teams.
	// The blue team player moves to red, and the last red player (highest PlayerIndex)
	// moves to blue. This gives every red player a chance to be blue in round robin order.
	if (GState->WinningPlayer == -2)
	{
		ANetPlayerState* LastRedPlayer = nullptr;
		ANetPlayerState* BluePlayer = nullptr;

		for (APlayerController* Player : AllPlayers)
		{
			ANetPlayerState* State = Player->GetPlayerState<ANetPlayerState>();
			if (State->TeamID == EPlayerTeam::TEAM_Blue)
			{
				BluePlayer = State;
			}
			else if (State->TeamID == EPlayerTeam::TEAM_Red)
			{
				// Pick the red player with the highest PlayerIndex (last joined red)
				if (LastRedPlayer == nullptr || State->PlayerIndex > LastRedPlayer->PlayerIndex)
				{
					LastRedPlayer = State;
				}
			}
		}

		// Swap teams between blue player and last red player
		if (BluePlayer && LastRedPlayer)
		{
			BluePlayer->TeamID = EPlayerTeam::TEAM_Red;
			LastRedPlayer->TeamID = EPlayerTeam::TEAM_Blue;
		}
	}

	GState->WinningPlayer = -1;

	for (APlayerController* Player : AllPlayers)
	{
		APawn* Pawn = Player->GetPawn();
		Player->UnPossess();
		Pawn->Destroy();
		Player->StartSpot.Reset();
		RestartPlayer(Player);
	}

	GState->TriggerRestart();

	// Restart the match timer for the new round
	GState->StartMatchTimer();
}

void ANetGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Start match timer when first player joins
	// Blue team wins if they survive 30 seconds
	if (TotalPlayerCount == 1)
	{
		ANetGameState* GState = GetGameState<ANetGameState>();
		if (GState)
		{
			GState->StartMatchTimer();
		}
	}
}