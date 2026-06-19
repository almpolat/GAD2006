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

    // HUD widget sýnýfý (Blueprint'te atanacak)
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> HUDWidgetClass;

    // EndScreen widget sýnýfý (Blueprint'te atanacak)
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> EndScreenWidgetClass;

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowHUD();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowEndScreen(bool bVictory);

    UFUNCTION(Client, Reliable, BlueprintCallable, Category = "UI")
    void ClientShowEndScreen(bool bVictory);

private:
    UPROPERTY()
    UUserWidget* HUDWidgetInstance;

    UPROPERTY()
    UUserWidget* EndScreenWidgetInstance;
};