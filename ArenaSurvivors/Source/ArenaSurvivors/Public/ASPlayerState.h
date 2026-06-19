#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ASPlayerState.generated.h"

UCLASS()
class ARENASURVIVORS_API AASPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    AASPlayerState();

    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Skor
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
    int32 PlayerScore;

    // Kill sayýsý
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
    int32 KillCount;

    // Nickname (GameInstance'tan kopyalanacak)
    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Player")
    FString PlayerNickname;

    // Kostüm indexi (GameInstance'tan kopyalanacak)
    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Player")
    int32 CostumeIndex;

    // Skor ekle
    UFUNCTION(BlueprintCallable, Category = "Player")
    void AddScore(int32 Amount);

    // Kill ekle
    UFUNCTION(BlueprintCallable, Category = "Player")
    void AddKill();
};