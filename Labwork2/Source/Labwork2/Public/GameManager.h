#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameSlot.h"
#include "Command.h"
#include "UnitBase.h"
#include "GameManager.generated.h"

class AGameGrid;

USTRUCT(BlueprintType)
struct FSUnitInfo
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AUnitBase> UnitClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSGridPosition StartPosition;
};

USTRUCT(BlueprintType)
struct FSLevelInfo
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FSUnitInfo> Units;
};

UCLASS()
class LABWORK2_API AGameManager : public AActor
{
    GENERATED_BODY()
public:
    AGameManager();

    UPROPERTY(EditAnywhere)
    TArray<FSLevelInfo> Levels;

    UPROPERTY(EditAnywhere)
    int CurrentLevel;

    UPROPERTY(EditAnywhere)
    AGameGrid* GameGrid;

    UPROPERTY()
    AUnitBase* ThePlayer;

    TArray<TSharedRef<Command>> CommandPool;
    TSharedPtr<Command> CurrentCommand;

    void OnActorClicked(AActor* Actor, FKey button);

    UFUNCTION(BlueprintCallable)
    bool UndoLastMove();

    void CreateLevelActors(FSLevelInfo& Info);

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};