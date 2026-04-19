#include "GameManager.h"
#include "TBPlayerController.h"
#include "MoveCommand.h"
#include "GameSlot.h"

AGameManager::AGameManager()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentLevel = 0;
    ThePlayer = nullptr;
}

void AGameManager::BeginPlay()
{
    Super::BeginPlay();
    if (auto PlayerController = GetWorld()->GetFirstPlayerController<ATBPlayerController>())
    {
        PlayerController->GameManager = this;
    }
    if (Levels.IsValidIndex(CurrentLevel))
    {
        CreateLevelActors(Levels[CurrentLevel]);
    }
}

void AGameManager::OnActorClicked(AActor* Actor, FKey button)
{
    if (CurrentCommand.IsValid() && CurrentCommand->IsExecuting()) return;
    
    AGameSlot* Slot = Cast<AGameSlot>(Actor);
    if (!Slot) return;
    
    if (!ThePlayer)
    {
        UE_LOG(LogTemp, Error, TEXT("No Player Unit Detected!"));
        return;
    }
    
    if (!ThePlayer->Slot)
    {
        UE_LOG(LogTemp, Error, TEXT("Player has no slot assigned!"));
        return;
    }
    
    if (Slot->Unit == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Moving from (%d,%d) to (%d,%d)"), 
            ThePlayer->Slot->GridPosition.Row, ThePlayer->Slot->GridPosition.Col,
            Slot->GridPosition.Row, Slot->GridPosition.Col);
        
        TSharedRef<MoveCommand> Cmd = MakeShared<MoveCommand>(
            ThePlayer->Slot->GridPosition, 
            Slot->GridPosition, 
            GameGrid
        );
        
        CommandPool.Add(Cmd);
        Cmd->Execute();
        CurrentCommand = Cmd;
    }
}

void AGameManager::CreateLevelActors(FSLevelInfo& Info)
{
    ThePlayer = nullptr;
    
    UE_LOG(LogTemp, Warning, TEXT("=== Creating Level Actors ==="));
    UE_LOG(LogTemp, Warning, TEXT("Unit count: %d"), Info.Units.Num());
    
    for (const auto& UnitInfo : Info.Units)
    {
        if (AGameSlot* Slot = GameGrid->GetSlot(UnitInfo.StartPosition))
        {
            UE_LOG(LogTemp, Warning, TEXT("Spawning at Row:%d Col:%d"), 
                UnitInfo.StartPosition.Row, UnitInfo.StartPosition.Col);
            
            Slot->SpawnUnitHere(UnitInfo.UnitClass);
            
            if (Slot->Unit && Slot->Unit->IsControlledByThePlayer())
            {
                ThePlayer = Slot->Unit;
                UE_LOG(LogTemp, Warning, TEXT("Player unit found!"));
            }
        }
    }
    
    if (!ThePlayer)
    {
        UE_LOG(LogTemp, Error, TEXT("NO PLAYER UNIT WAS SET!"));
    }
}

bool AGameManager::UndoLastMove()
{
    if (CommandPool.Num() == 0) return false;
    CommandPool.Pop()->Revert();
    return true;
}

void AGameManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (CurrentCommand.IsValid())
    {
        CurrentCommand->Update(DeltaTime);
    }
}