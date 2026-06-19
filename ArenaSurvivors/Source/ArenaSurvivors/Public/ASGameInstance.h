#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ASGameInstance.generated.h"

UCLASS()
class ARENASURVIVORS_API UASGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UASGameInstance();

    // Oyuncu bilgileri
    UPROPERTY(BlueprintReadWrite, Category = "Player Info")
    FString PlayerNickname;

    UPROPERTY(BlueprintReadWrite, Category = "Player Info")
    int32 CostumeIndex;

    // Host / Join
    UFUNCTION(BlueprintCallable, Category = "Network")
    void HostGame();

    UFUNCTION(BlueprintCallable, Category = "Network")
    void JoinGame(const FString& IPAddress);

    // Level travel
    UFUNCTION(BlueprintCallable, Category = "Network")
    void TravelToArena();
};