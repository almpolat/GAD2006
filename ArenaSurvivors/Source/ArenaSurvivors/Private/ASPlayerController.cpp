#include "ASPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"

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
        // bIsVictory deðiþkenini direkt set et
        FBoolProperty* VictoryProp = FindFProperty<FBoolProperty>(
            EndScreenWidgetInstance->GetClass(), TEXT("bIsVictory"));
        if (VictoryProp)
        {
            VictoryProp->SetPropertyValue_InContainer(EndScreenWidgetInstance, bVictory);
        }

        // Viewport'a ekle — Event Construct burada tetiklenir
        EndScreenWidgetInstance->AddToViewport();
    }

    SetShowMouseCursor(true);
    SetInputMode(FInputModeUIOnly());
}