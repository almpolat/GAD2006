#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NetGameInstance.h"
#include "NetGameMode.generated.h"

class ANetAvatar;
class ANetPlayerState;

UCLASS()
class LABWORK4_API ANetGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ANetGameMode();

    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

    UFUNCTION(BlueprintCallable)
    void AvatarsOverlapped(ANetAvatar* AvatarA, ANetAvatar* AvatarB);

    UFUNCTION(BlueprintCallable)
    void EndGame();

    UFUNCTION(BlueprintCallable)
    void StartMatchTimer();

    void OnMatchTimerTick();

private:
    int TotalPlayerCount;
    int TotalGames;
    int PlayerStartIndex;

    TArray<APlayerController*> AllPlayers;

    FTimerHandle MatchTimerHandle;

    AActor* GetPlayerStart(FString Name, int Index);
    AActor* AssignTeamAndPlayerStart(AController* Player);
};