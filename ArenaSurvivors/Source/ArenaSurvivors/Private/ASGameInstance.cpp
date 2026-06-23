#include "ASGameInstance.h"
#include "Engine/Engine.h"

UASGameInstance::UASGameInstance()
{
    PlayerNickname = TEXT("Player");
    CostumeIndex = 0;
    CostumeRowName = FName("Costume1");
}

void UASGameInstance::HostGame()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Hosting game..."));
    }

    UWorld* World = GetWorld();
    if (World)
    {
        World->ServerTravel(TEXT("/Game/Maps/ArenaMap?listen"));
    }
}

void UASGameInstance::JoinGame(const FString& IPAddress)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            FString::Printf(TEXT("Joining: %s"), *IPAddress));
    }

    APlayerController* PC = GetFirstLocalPlayerController();
    if (PC)
    {
        PC->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
    }
}

void UASGameInstance::TravelToArena()
{
    UWorld* World = GetWorld();
    if (World)
    {
        World->ServerTravel(TEXT("/Game/Maps/ArenaMap?listen"));
    }
}