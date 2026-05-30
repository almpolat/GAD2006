#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NetGameState.generated.h"

class ANetPlayerState;

UCLASS()
class LABWORK4_API ANetGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ANetGameState();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Winner)
	int WinningPlayer;

	UPROPERTY(BlueprintReadOnly, Replicated)
	float MatchTimeRemaining;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bMatchTimerActive;

	UFUNCTION(BlueprintCallable)
	void StartMatchTimer();

	UFUNCTION()
	void OnMatchTimerTick();

	FTimerHandle MatchTimerHandle;

	UFUNCTION()
	void OnRep_Winner();

	UFUNCTION(BlueprintImplementableEvent)
	void OnVictory();

	UFUNCTION(BlueprintImplementableEvent)
	void OnRestart();

	UFUNCTION(NetMulticast, Reliable)
	void TriggerRestart();

	UFUNCTION(BlueprintCallable)
	ANetPlayerState* GetPlayerStateByIndex(int PlayerIndex);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};