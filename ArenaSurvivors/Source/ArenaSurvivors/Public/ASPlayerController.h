#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ASPlayerController.generated.h"

UCLASS()
class ARENASURVIVORS_API AASPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AASPlayerController();

    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> HUDWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> EndScreenWidgetClass;

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowHUD();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowEndScreen(bool bVictory);

    UFUNCTION(Client, Reliable, BlueprintCallable, Category = "UI")
    void ClientShowEndScreen(bool bVictory);

    UFUNCTION(Client, Reliable, Category = "Spectator")
    void ClientSpectatePlayer(AActor* Target);

    // End-game sonuçlarýný client GameInstance'a yazar
    UFUNCTION(Client, Reliable, Category = "EndGame")
    void Client_StoreEndGameResults(bool bVictory, int32 WaveReached,
        int32 TotalKills, int32 MeleeKills, int32 RangedKills);

private:
    UPROPERTY()
    UUserWidget* HUDWidgetInstance;

    UPROPERTY()
    UUserWidget* EndScreenWidgetInstance;
};