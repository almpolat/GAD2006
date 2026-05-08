#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitBase.generated.h"

class AGameSlot;

UCLASS()
class LABWORK2_API AUnitBase : public AActor
{
    GENERATED_BODY()
public:
    AUnitBase();

    UPROPERTY(EditAnywhere)
    FVector StartOffset;

    UPROPERTY()
    AGameSlot* Slot;

    void AssignToSlot(AGameSlot* NewSlot);

    UFUNCTION(BlueprintImplementableEvent)
    bool IsControlledByThePlayer();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};