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
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PlayerInfo)
	FSPlayerInfo Data;

	UPROPERTY(BlueprintReadOnly, Replicated)
	int PlayerIndex;

	EPlayerTeam TeamID;
	EGameResults Result;

private:
	UFUNCTION()
	void OnRep_PlayerInfo();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};