#include "UnitBase.h"
#include "GameSlot.h"  // .cpp'de include

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
    
    // Eğer eski slotumuz varsa, onu temizle
    if (Slot)
    {
        Slot->Unit = nullptr;
    }
    
    // Yeni slota geç
    Slot = NewSlot;
    Slot->Unit = this;
    
    // Konumu güncelle
    SetActorLocation(Slot->GetActorLocation() + StartOffset);
}

void AUnitBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}