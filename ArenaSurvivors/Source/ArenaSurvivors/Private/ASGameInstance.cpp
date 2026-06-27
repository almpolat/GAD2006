#include "ASGameInstance.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

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
        GEngine->AddOnScreenDebugMessage(
            -1,
            8.f,
            FColor::Green,
            TEXT("HostGame called - opening ArenaMap as listen server...")
        );
    }

    UWorld* World = GetWorld();

    if (!World)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                8.f,
                FColor::Red,
                TEXT("HostGame failed: World is null!")
            );
        }
        return;
    }

    World->ServerTravel(TEXT("/Game/Maps/ArenaMap?listen"));
}

void UASGameInstance::JoinGame(const FString& IPAddress)
{
    FString FinalAddress = IPAddress.TrimStartAndEnd();

    if (FinalAddress.IsEmpty())
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                8.f,
                FColor::Red,
                TEXT("JoinGame failed: IP address is empty!")
            );
        }
        return;
    }

    if (!FinalAddress.Contains(TEXT(":")))
    {
        FinalAddress += TEXT(":7777");
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            8.f,
            FColor::Yellow,
            FString::Printf(TEXT("JoinGame called with IP: %s"), *FinalAddress)
        );
    }

    APlayerController* PC = GetFirstLocalPlayerController();

    if (!PC)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                8.f,
                FColor::Red,
                TEXT("JoinGame failed: PlayerController is null!")
            );
        }
        return;
    }

    PC->ClientTravel(FinalAddress, ETravelType::TRAVEL_Absolute);
}

void UASGameInstance::TravelToArena()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            8.f,
            FColor::Cyan,
            TEXT("TravelToArena called...")
        );
    }

    UWorld* World = GetWorld();

    if (!World)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                8.f,
                FColor::Red,
                TEXT("TravelToArena failed: World is null!")
            );
        }
        return;
    }

    World->ServerTravel(TEXT("/Game/Maps/ArenaMap?listen"));
}