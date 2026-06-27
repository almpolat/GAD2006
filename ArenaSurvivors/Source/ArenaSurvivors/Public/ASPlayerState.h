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

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Stats")
    int32 PlayerScore;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Stats")
    int32 KillCount;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Player")
    FString PlayerNickname;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Player")
    int32 CostumeIndex;

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void AddScore(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void AddKill();
};