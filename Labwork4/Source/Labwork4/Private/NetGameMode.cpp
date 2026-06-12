#include "NetGameMode.h"
#include "NetBaseCharacter.h"
#include "NetGameState.h"
#include "NetPlayerState.h"
#include "NetAvatar.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Components/CapsuleComponent.h"

ANetGameMode::ANetGameMode() : TotalPlayerCount(0), TotalGames(0), PlayerStartIndex(0)
{
    DefaultPawnClass = ANetBaseCharacter::StaticClass();
    PlayerStateClass = ANetPlayerState::StaticClass();
    GameStateClass = ANetGameState::StaticClass();
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
            State->TeamID = State->Result == EGameResult::RESULT_Won ?
                EPlayerTeam::TEAM_Blue : EPlayerTeam::TEAM_Red;
        }

        if (State->TeamID == EPlayerTeam::TEAM_Blue)
            Start = GetPlayerStart("Blue", -1);
        else
            Start = GetPlayerStart("Red", PlayerStartIndex++);
    }

    return Start;
}

AActor* ANetGameMode::GetPlayerStart(FString Name, int Index)
{
    FName PSName;
    if (Index < 0)
        PSName = *Name;
    else
        PSName = *FString::Printf(TEXT("%s%d"), *Name, Index % 4);

    for (TActorIterator<APlayerStart> It(GWorld); It; ++It)
    {
        if (APlayerStart* PS = Cast<APlayerStart>(*It))
            if (PS->PlayerStartTag == PSName) return *It;
    }
    return nullptr;
}

void ANetGameMode::AvatarsOverlapped(ANetAvatar* AvatarA, ANetAvatar* AvatarB)
{
    // ── NULL GUARD ──────────────────────────────────────────────────────────
    if (!AvatarA || !AvatarB) return;

    ANetGameState* GState = GetGameState<ANetGameState>();

    // WinningPlayer != -1 → hem >= 0 (kırmızı kazandı) hem -2 (mavi timer)
    // durumunda çift tetiklemeyi önler
    if (!GState || GState->WinningPlayer != -1) return;

    ANetPlayerState* StateA = AvatarA->GetPlayerState<ANetPlayerState>();
    ANetPlayerState* StateB = AvatarB->GetPlayerState<ANetPlayerState>();

    // PlayerState henüz hazır değilse crash önle
    if (!StateA || !StateB) return;

    if (StateA->TeamID == StateB->TeamID) return;

    if (StateA->TeamID == EPlayerTeam::TEAM_Red)
        GState->WinningPlayer = StateA->PlayerIndex;
    else
        GState->WinningPlayer = StateB->PlayerIndex;

    AvatarA->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    AvatarB->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

    GState->OnVictory();

    for (APlayerController* Player : AllPlayers)
    {
        auto State = Player->GetPlayerState<ANetPlayerState>();
        if (State)
        {
            if (State->TeamID == EPlayerTeam::TEAM_Blue)
                State->Result = EGameResult::RESULT_Lost;
            else
                State->Result = EGameResult::RESULT_Won;
        }
    }

    FTimerHandle EndGameTimerHandle;
    GWorld->GetTimerManager().SetTimer(EndGameTimerHandle, this,
        &ANetGameMode::EndGame, 2.5f, false);
}

void ANetGameMode::StartMatchTimer()
{
    ANetGameState* GState = GetGameState<ANetGameState>();
    if (!GState) return;

    GState->MatchTimeRemaining = 30.0f;
    GState->bMatchTimerActive = true;

    GWorld->GetTimerManager().SetTimer(MatchTimerHandle, this,
        &ANetGameMode::OnMatchTimerTick, 1.0f, true);
}

void ANetGameMode::OnMatchTimerTick()
{
    ANetGameState* GState = GetGameState<ANetGameState>();
    if (!GState) return;

    GState->MatchTimeRemaining -= 1.0f;

    if (GState->MatchTimeRemaining <= 0)
    {
        GWorld->GetTimerManager().ClearTimer(MatchTimerHandle);
        GState->bMatchTimerActive = false;

        if (GState->WinningPlayer != -1) return;

        GState->WinningPlayer = -2;
        GState->OnVictory();

        for (APlayerController* Player : AllPlayers)
        {
            auto State = Player->GetPlayerState<ANetPlayerState>();
            if (State)
            {
                if (State->TeamID == EPlayerTeam::TEAM_Blue)
                    State->Result = EGameResult::RESULT_Won;
                else
                    State->Result = EGameResult::RESULT_Lost;
            }
        }

        FTimerHandle EndGameTimerHandle;
        GWorld->GetTimerManager().SetTimer(EndGameTimerHandle, this,
            &ANetGameMode::EndGame, 2.5f, false);
    }
}

void ANetGameMode::EndGame()
{
    GWorld->GetTimerManager().ClearTimer(MatchTimerHandle);

    PlayerStartIndex = 0;
    TotalGames++;

    ANetGameState* GState = GetGameState<ANetGameState>();
    if (GState)
    {
        GState->WinningPlayer = -1;
        GState->MatchTimeRemaining = 30.0f;
    }

    for (APlayerController* Player : AllPlayers)
    {
        APawn* OldPawn = Player->GetPawn();
        Player->UnPossess();
        if (OldPawn) OldPawn->Destroy();
        Player->StartSpot = nullptr;
        RestartPlayer(Player);
    }

    if (GState) GState->TriggerRestart();
}