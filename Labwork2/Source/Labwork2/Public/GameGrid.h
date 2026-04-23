#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameSlot.h"
#include "GameGrid.generated.h"

UCLASS()
class LABWORK2_API AGameGrid : public AActor
{
    GENERATED_BODY()
public:
    AGameGrid();
    UPROPERTY(EditAnywhere)
    int NumRows;
    UPROPERTY(EditAnywhere)
    int NumCols;
    UPROPERTY(EditAnywhere)
    float SlotSize;
    UPROPERTY(EditAnywhere)
    TSubclassOf<AGameSlot> GridClass;
    TArray<UChildActorComponent*> GridActors;
    static AGameSlot* FindSlot(FSGridPosition Position);
    AGameSlot* GetSlot(FSGridPosition Position);
protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;
public:
    virtual void Tick(float DeltaTime) override;
private:
    static AGameGrid* GameGrid;
};