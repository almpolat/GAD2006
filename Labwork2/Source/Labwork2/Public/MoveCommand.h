#pragma once
#include "Command.h"
#include "GameGrid.h"
#include "GameSlot.h"

class MoveCommand : public Command
{
public:
    MoveCommand(FSGridPosition Src, FSGridPosition Dst, AGameGrid* Grid);
    ~MoveCommand();
    void Execute() override;
    void Revert() override;

private:
    FSGridPosition Source;
    FSGridPosition Destination;
    AGameGrid* GameGrid;
};