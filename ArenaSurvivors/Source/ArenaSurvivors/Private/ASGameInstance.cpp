#include "ASGameInstance.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

UASGameInstance::UASGameInstance()
{
    PlayerNickname = TEXT("Player");
    CostumeIndex = 0;
    CostumeRowName = FName("Costume1");

    bLastGameVictory = false;
    LastWaveReached = 0;
    LastTotalKills = 0;
    LastMeleeKills = 0;
    LastRangedKills = 0;
}

void UASGameInstance::HostGame()
{
    UWorld* World = GetWorld();
    if (!World) return;
    World->ServerTravel(TEXT("/Game/Maps/ArenaMap?listen"));
}

void UASGameInstance::JoinGame(const FString& IPAddress)
{
    FString FinalAddress = IPAddress.TrimStartAndEnd();
    if (FinalAddress.IsEmpty()) return;
    if (!FinalAddress.Contains(TEXT(":")))
        FinalAddress += TEXT(":7777");

    APlayerController* PC = GetFirstLocalPlayerController();
    if (!PC) return;
    PC->ClientTravel(FinalAddress, ETravelType::TRAVEL_Absolute);
}

void UASGameInstance::TravelToArena()
{
    UWorld* World = GetWorld();
    if (!World) return;
    World->ServerTravel(TEXT("/Game/Maps/ArenaMap?listen"));
}