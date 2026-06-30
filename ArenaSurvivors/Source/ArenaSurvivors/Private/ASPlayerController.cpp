#include "ASPlayerController.h"
#include "ASGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"

AASPlayerController::AASPlayerController()
{
    HUDWidgetInstance = nullptr;
    EndScreenWidgetInstance = nullptr;
}

void AASPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalController())
    {
        ShowHUD();
    }
}

void AASPlayerController::ShowHUD()
{
    if (!HUDWidgetClass) return;

    if (!HUDWidgetInstance)
    {
        HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
    }

    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->AddToViewport();
    }
}

void AASPlayerController::ShowEndScreen(bool bVictory)
{
    ClientShowEndScreen(bVictory);
}

void AASPlayerController::ClientShowEndScreen_Implementation(bool bVictory)
{
    if (!EndScreenWidgetClass) return;

    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
    }

    if (!EndScreenWidgetInstance)
    {
        EndScreenWidgetInstance = CreateWidget<UUserWidget>(this, EndScreenWidgetClass);
    }

    if (EndScreenWidgetInstance)
    {
        FBoolProperty* VictoryProp = FindFProperty<FBoolProperty>(
            EndScreenWidgetInstance->GetClass(), TEXT("bIsVictory"));
        if (VictoryProp)
        {
            VictoryProp->SetPropertyValue_InContainer(EndScreenWidgetInstance, bVictory);
        }

        EndScreenWidgetInstance->AddToViewport();
    }

    SetShowMouseCursor(true);
    SetInputMode(FInputModeUIOnly());
}

void AASPlayerController::ClientSpectatePlayer_Implementation(AActor* Target)
{
    if (!Target) return;

    SetIgnoreMoveInput(true);
    SetIgnoreLookInput(true);
    SetViewTargetWithBlend(Target, 1.0f, EViewTargetBlendFunction::VTBlend_Cubic);
}

void AASPlayerController::Client_StoreEndGameResults_Implementation(
    bool bVictory, int32 WaveReached, int32 TotalKills, int32 MeleeKills, int32 RangedKills)
{
    UASGameInstance* GI = Cast<UASGameInstance>(GetGameInstance());
    if (!GI) return;

    GI->bLastGameVictory = bVictory;
    GI->LastWaveReached = WaveReached;
    GI->LastTotalKills = TotalKills;
    GI->LastMeleeKills = MeleeKills;
    GI->LastRangedKills = RangedKills;
}