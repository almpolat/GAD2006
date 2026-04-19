#include "GameGrid.h"
#include "Components/ChildActorComponent.h"

AGameGrid* AGameGrid::GameGrid = nullptr;

AGameGrid::AGameGrid()
{
    PrimaryActorTick.bCanEverTick = true;

    NumRows = 8;
    NumCols = 8;
    SlotSize = 100.0f;
    GameGrid = this;
}

void AGameGrid::BeginPlay()
{
    Super::BeginPlay();

    GridActors.Empty();

    for (int i = 0; i < NumRows; i++)
    {
        for (int j = 0; j < NumCols; j++)
        {
            FName GridName(FString::Printf(TEXT("Slot%dx%d"), j, i));
            UChildActorComponent* Found = nullptr;

            TArray<UChildActorComponent*> Components;
            GetComponents<UChildActorComponent>(Components);
            for (auto* Comp : Components)
            {
                if (Comp->GetFName() == GridName)
                {
                    Found = Comp;
                    break;
                }
            }

            GridActors.Add(Found);

            if (Found)
            {
                AGameSlot* Slot = Cast<AGameSlot>(Found->GetChildActor());
                if (Slot)
                {
                    Slot->GridPosition.Row = i;
                    Slot->GridPosition.Col = j;
                }
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("GameGrid BeginPlay - GridActors sayisi: %d"), GridActors.Num());
}

void AGameGrid::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    for (auto Grid : GridActors)
    {
        if (Grid) Grid->DestroyComponent();
    }
    GridActors.Empty();

    for (int i = 0; i < NumRows; i++)
    {
        for (int j = 0; j < NumCols; j++)
        {
            FName GridName(FString::Printf(TEXT("Slot%dx%d"), j, i));

            UChildActorComponent* NewSlot = NewObject<UChildActorComponent>(
                this, GridName);
            NewSlot->RegisterComponent();
            NewSlot->SetupAttachment(RootComponent);
            NewSlot->SetChildActorClass(GridClass);

            FVector Location(j * SlotSize, i * SlotSize, 0.0f);
            NewSlot->SetRelativeLocation(Location);

            GridActors.Add(NewSlot);
        }
    }
}

void AGameGrid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

AGameSlot* AGameGrid::FindSlot(FSGridPosition Position)
{
    if (GameGrid)
        return GameGrid->GetSlot(Position);
    return nullptr;
}

AGameSlot* AGameGrid::GetSlot(FSGridPosition Position)
{
    int Index = Position.Row * NumCols + Position.Col;
    if (GridActors.IsValidIndex(Index))
        return Cast<AGameSlot>(GridActors[Index]->GetChildActor());
    return nullptr;
}