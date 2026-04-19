#include "MoveCommand.h"
#include "UnitBase.h"

MoveCommand::MoveCommand(FSGridPosition Src, FSGridPosition Dst, AGameGrid* Grid)
    : Source(Src), Destination(Dst), GameGrid(Grid)
{
}

MoveCommand::~MoveCommand()
{
}

void MoveCommand::Execute()
{
    AGameSlot* SourceSlot = GameGrid->GetSlot(Source);
    AGameSlot* DestSlot = GameGrid->GetSlot(Destination);
    
    if (!SourceSlot || !DestSlot)
    {
        UE_LOG(LogTemp, Error, TEXT("Execute: Invalid slots!"));
        return;
    }
    
    if (!SourceSlot->Unit)
    {
        UE_LOG(LogTemp, Error, TEXT("Execute: No unit in source slot!"));
        return;
    }
    
    // Hedef slotta zaten bir unit varsa hareket etme
    if (DestSlot->Unit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Execute: Destination slot occupied!"));
        return;
    }
    
    SourceSlot->Unit->AssignToSlot(DestSlot);
    DestSlot->SetState(EGridState::GS_Highlighted);
    SourceSlot->SetState(EGridState::GS_Default);
}

void MoveCommand::Revert()
{
    AGameSlot* SourceSlot = GameGrid->GetSlot(Source);
    AGameSlot* DestSlot = GameGrid->GetSlot(Destination);
    
    if (!SourceSlot || !DestSlot)
    {
        UE_LOG(LogTemp, Error, TEXT("Revert: Invalid slots!"));
        return;
    }
    
    if (!DestSlot->Unit)
    {
        UE_LOG(LogTemp, Error, TEXT("Revert: No unit in destination slot!"));
        return;
    }
    
    // Source slotta unit varsa hareket etme (çakışma olmamalı)
    if (SourceSlot->Unit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Revert: Source slot occupied!"));
        return;
    }
    
    DestSlot->Unit->AssignToSlot(SourceSlot);
    SourceSlot->SetState(EGridState::GS_Default);
    DestSlot->SetState(EGridState::GS_Default);
}