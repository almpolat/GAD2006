#include "GameSlot.h"
#include "UnitBase.h"
#include "TBPlayerController.h"

AGameSlot::AGameSlot()
{
    PrimaryActorTick.bCanEverTick = true;

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    RootComponent = Box;

    Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
    Plane->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane.Plane"));
    if (PlaneMesh.Succeeded())
    {
        Plane->SetStaticMesh(PlaneMesh.Object);
    }

    GridState = EGridState::GS_Default;
    Unit = nullptr;
}

void AGameSlot::BeginPlay()
{
    Super::BeginPlay();
    OnClicked.AddDynamic(this, &AGameSlot::OnGridClicked);
}

void AGameSlot::SetState(EGridState NewState)
{
    GridState = NewState;
    UMaterialInterface* TargetMat = nullptr;
    switch (NewState)
    {
    case EGridState::GS_Default: TargetMat = Mat_Default; break;
    case EGridState::GS_Highlighted: TargetMat = Mat_Highlighted; break;
    case EGridState::GS_Offensive: TargetMat = Mat_Offensive; break;
    case EGridState::GS_Supportive: TargetMat = Mat_Supportive; break;
    }
    if (TargetMat && Plane)
    {
        Plane->SetMaterial(0, TargetMat);
    }
}

void AGameSlot::SpawnUnitHere(TSubclassOf<AUnitBase> UnitClass)
{
    if (!UnitClass) return;

    FActorSpawnParameters SpawnParams;
    AUnitBase* NewUnit = GetWorld()->SpawnActor<AUnitBase>(
        UnitClass, GetActorLocation(), GetActorRotation(), SpawnParams);

    if (NewUnit)
    {
        NewUnit->AssignToSlot(this);
    }
}

void AGameSlot::OnGridClicked(AActor* TouchedActor, FKey ButtonPressed)
{
    if (auto PC = GetWorld()->GetFirstPlayerController<ATBPlayerController>())
    {
        PC->OnActorClicked(this, ButtonPressed);
    }
}

void AGameSlot::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}