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

    UPROPERTY(BlueprintReadWrite, Category = "Player Info")
    FName CostumeRowName;

    // End-game sonuçlarý
    UPROPERTY(BlueprintReadWrite, Category = "End Game")
    bool bLastGameVictory;

    UPROPERTY(BlueprintReadWrite, Category = "End Game")
    int32 LastWaveReached;

    UPROPERTY(BlueprintReadWrite, Category = "End Game")
    int32 LastTotalKills;

    UPROPERTY(BlueprintReadWrite, Category = "End Game")
    int32 LastMeleeKills;

    UPROPERTY(BlueprintReadWrite, Category = "End Game")
    int32 LastRangedKills;

    UFUNCTION(BlueprintCallable, Category = "Network")
    void HostGame();

    UFUNCTION(BlueprintCallable, Category = "Network")
    void JoinGame(const FString& IPAddress);

    UFUNCTION(BlueprintCallable, Category = "Network")
    void TravelToArena();
};