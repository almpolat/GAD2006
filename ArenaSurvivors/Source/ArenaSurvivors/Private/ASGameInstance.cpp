#include "ASGameInstance.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

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

    GWorld->ServerTravel(TEXT("/Game/Maps/ArenaMap?listen"));
}

void UASGameInstance::JoinGame(const FString& IPAddress)
{
    if (IPAddress.IsEmpty())
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Join failed: IP address is empty"));
        }
        return;
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            FString::Printf(TEXT("Joining: %s"), *IPAddress));
    }

    GWorld->GetFirstPlayerController()->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
}

void UASGameInstance::TravelToArena()
{
    GWorld->ServerTravel(TEXT("/Game/Maps/ArenaMap?listen"));
}