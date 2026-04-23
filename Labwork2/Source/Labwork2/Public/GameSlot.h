#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GameSlot.generated.h"

class AUnitBase;

USTRUCT(BlueprintType)
struct FSGridPosition
{
    GENERATED_USTRUCT_BODY()

    FSGridPosition() {}
    FSGridPosition(int col, int row) : Col(col), Row(row) {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 Col;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 Row;
};

UENUM(BlueprintType)
enum class EGridState : uint8
{
    GS_Default,
    GS_Highlighted,
    GS_Offensive,
    GS_Supportive,
};

UCLASS()
class LABWORK2_API AGameSlot : public AActor
{
    GENERATED_BODY()

public:
    AGameSlot();

    UPROPERTY(EditAnywhere)
    UBoxComponent* Box;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* Plane;

    UPROPERTY(BlueprintReadWrite)
    FSGridPosition GridPosition;

    UPROPERTY(VisibleAnywhere)
    AUnitBase* Unit;

    UFUNCTION()
    void SetState(EGridState NewState);

    void SpawnUnitHere(TSubclassOf<AUnitBase> UnitClass);

    UFUNCTION()
    void OnGridClicked(AActor* TouchedActor, FKey ButtonPressed);

protected:
    virtual void BeginPlay() override;

private:
    EGridState GridState;

    UPROPERTY()
    UMaterialInterface* Mat_Default;
    UPROPERTY()
    UMaterialInterface* Mat_Highlighted;
    UPROPERTY()
    UMaterialInterface* Mat_Offensive;
    UPROPERTY()
    UMaterialInterface* Mat_Supportive;

public:
    virtual void Tick(float DeltaTime) override;
};