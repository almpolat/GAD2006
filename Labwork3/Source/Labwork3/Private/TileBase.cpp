#include "TileBase.h"

// Sets default values
ATileBase::ATileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	InstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMesh"));
	InstancedMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CubeMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	InstancedMesh->SetStaticMesh(CubeMesh.Object);
	InstancedMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATileBase::OnConstruction(const FTransform& Transform)
{
	if (!BaseMesh) return;
	InstancedMesh->SetStaticMesh(BaseMesh);
}

// Called when the game starts or when spawned
void ATileBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}