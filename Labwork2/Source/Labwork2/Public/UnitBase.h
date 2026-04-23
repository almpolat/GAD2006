#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitBase.generated.h"

class AGameSlot;  // Forward declare — circular dependency fix

UCLASS()
class LABWORK2_API AUnitBase : public AActor
{
    GENERATED_BODY()

public:
    AUnitBase();

    void AssignToSlot(AGameSlot* NewSlot);

    UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
    bool IsControlledByThePlayer();

    UPROPERTY(EditAnywhere)
    FVector StartOffset;

    UPROPERTY(VisibleAnywhere)
    AGameSlot* Slot;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};