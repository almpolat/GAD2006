#include "GameSlot.h"
#include "UnitBase.h"
#include "TBPlayerController.h"

AGameSlot::AGameSlot()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    Box->SetupAttachment(RootComponent);
    Box->SetBoxExtent(FVector(50, 50, 2));
    Box->SetCollisionResponseToAllChannels(ECR_Block);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultSlotMesh(TEXT("/Engine/BasicShapes/Plane"));
    Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
    Plane->SetupAttachment(RootComponent);
    Plane->SetStaticMesh(DefaultSlotMesh.Object);

    static ConstructorHelpers::FObjectFinder<UMaterialInterface>
        M_Default(TEXT("Material'/Game/Materials/MAT_GridSlot.MAT_GridSlot'"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface>
        M_Highlighted(TEXT("MaterialInstanceConstant'/Game/Materials/MAT_GridSlot_Highlighted.MAT_GridSlot_Highlighted'"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface>
        M_Offensive(TEXT("MaterialInstanceConstant'/Game/Materials/MAT_GridSlot_Offensive.MAT_GridSlot_Offensive'"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface>
        M_Supportive(TEXT("MaterialInstanceConstant'/Game/Materials/MAT_GridSlot_Supportive.MAT_GridSlot_Supportive'"));

    Mat_Default = M_Default.Object;
    Mat_Highlighted = M_Highlighted.Object;
    Mat_Offensive = M_Offensive.Object;
    Mat_Supportive = M_Supportive.Object;

    Unit = nullptr;
}

void AGameSlot::BeginPlay()
{
    Super::BeginPlay();
    OnClicked.AddUniqueDynamic(this, &AGameSlot::OnGridClicked);
    SetState(EGridState::GS_Default);
}

void AGameSlot::SetState(EGridState NewState)
{
    GridState = NewState;

    switch (NewState)
    {
    case EGridState::GS_Default:
        if (Mat_Default) Plane->SetMaterial(0, Mat_Default);
        break;
    case EGridState::GS_Highlighted:
        if (Mat_Highlighted) Plane->SetMaterial(0, Mat_Highlighted);
        break;
    case EGridState::GS_Offensive:
        if (Mat_Offensive) Plane->SetMaterial(0, Mat_Offensive);
        break;
    case EGridState::GS_Supportive:
        if (Mat_Supportive) Plane->SetMaterial(0, Mat_Supportive);
        break;
    }
}

void AGameSlot::SpawnUnitHere(TSubclassOf<AUnitBase> UnitClass)
{
    FVector Location = GetActorLocation();
    AUnitBase* NewUnit = Cast<AUnitBase>(GetWorld()->SpawnActor(UnitClass, &Location));
    if (NewUnit) NewUnit->AssignToSlot(this);
}

void AGameSlot::OnGridClicked(AActor* TouchedActor, FKey ButtonPressed)
{
    if (auto PlayerController = GetWorld()->GetFirstPlayerController<ATBPlayerController>())
    {
        PlayerController->OnActorClicked(this, ButtonPressed);
    }
}

void AGameSlot::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}