#include "ASPlayerController.h"
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

    // Hareketi durdur
    SetIgnoreMoveInput(true);
    SetIgnoreLookInput(true);

    // Kamerayý yaþayan oyuncuya yönlendir
    SetViewTargetWithBlend(Target, 1.0f, EViewTargetBlendFunction::VTBlend_Cubic);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            TEXT("You are now spectating the other player."));
    }
}