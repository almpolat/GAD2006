#include "UnitBase.h"
#include "GameSlot.h"

AUnitBase::AUnitBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AUnitBase::BeginPlay()
{
    Super::BeginPlay();
}

void AUnitBase::AssignToSlot(AGameSlot* NewSlot)
{
    if (!NewSlot)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignToSlot: NewSlot is nullptr!"));
        return;
    }

    if (Slot)
    {
        Slot->Unit = nullptr;
    }

    Slot = NewSlot;
    Slot->Unit = this;

    SetActorLocation(Slot->GetActorLocation() + StartOffset);
}

void AUnitBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}