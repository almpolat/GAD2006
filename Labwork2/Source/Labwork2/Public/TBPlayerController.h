#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TBPlayerController.generated.h"

class AGameManager;

UCLASS()
class LABWORK2_API ATBPlayerController : public APlayerController
{
    GENERATED_BODY()
public:
    ATBPlayerController();

    UPROPERTY()
    AGameManager* GameManager;

    UFUNCTION()
    void OnActorClicked(AActor* Actor, FKey button);

protected:
    virtual void BeginPlay() override;
};