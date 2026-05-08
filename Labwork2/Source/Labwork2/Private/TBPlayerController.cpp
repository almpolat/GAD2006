#include "TBPlayerController.h"
#include "GameManager.h"

ATBPlayerController::ATBPlayerController()
{
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void ATBPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void ATBPlayerController::OnActorClicked(AActor* Actor, FKey button)
{
    UE_LOG(LogTemp, Warning, TEXT("OnClicked: %s - %s"),
        Actor ? *Actor->GetName() : TEXT("NULL"),
        *button.ToString());

    if (GameManager)
    {
        GameManager->OnActorClicked(Actor, button);
    }
}