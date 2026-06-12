#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NetGameInstance.h"
#include "NetPlayerState.generated.h"

UCLASS()
class LABWORK4_API ANetPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PlayerInfo)
    FSPlayerInfo Data;

    UPROPERTY(BlueprintReadOnly, Replicated)
    int PlayerIndex;

    UPROPERTY(BlueprintReadWrite)
    EPlayerTeam TeamID;

    UPROPERTY(BlueprintReadWrite)
    EGameResult Result;

private:
    UFUNCTION()
    void OnRep_PlayerInfo();
};