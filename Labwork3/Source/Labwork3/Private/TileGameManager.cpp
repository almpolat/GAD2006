#include "TileGameManager.h"
#include "TilePlayerController.h"

ATileGameManager::ATileGameManager() :
	GridSize(100),
	GridOffset(0, 0, 0.5f)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	GridSelection = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	GridSelection->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		PlaneMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		GridMaterial(TEXT("Material'/Game/UI/MAT_GridSlot.MAT_GridSlot'"));

	GridSelection->SetStaticMesh(PlaneMesh.Object);
	GridSelection->SetMaterial(0, GridMaterial.Object);
	GridSelection->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TilePreview = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TilePreview"));
	TilePreview->SetupAttachment(RootComponent);
	TilePreview->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MapExtendsInGrids = 25;
	CurrentTileIndex = 0;
	CurrentTileYaw = 0.0f;
}

void ATileGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (auto PlayerController =
		Cast<ATilePlayerController>(GWorld->GetFirstPlayerController()))
	{
		PlayerController->GameManager = this;
	}
}

void ATileGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATileGameManager::OnActorInteraction(AActor* HitActor, FVector& Location, bool IsPressed)
{
	// No tile types?
	if (TileTypes.Num() == 0) return;

	FVector GridLoc = GridOffset;
	GridLoc.X += FMath::GridSnap(Location.X, GridSize);
	GridLoc.Y += FMath::GridSnap(Location.Y, GridSize);
	GridLoc.Z += Location.Z;

	UPlayerInput* Input = GWorld->GetFirstPlayerController()->PlayerInput;

	if (Input->WasJustPressed(EKeys::LeftMouseButton))
	{
		int GridX = GridLoc.X / GridSize + MapExtendsInGrids;
		int GridY = GridLoc.Y / GridSize + MapExtendsInGrids;

		if (GridX < 0 || GridY < 0 || GridX >= MapExtendsInGrids * 2 || GridY >= MapExtendsInGrids * 2)
		{
			return;
		}

		// Already a tile here?
		if (Map[GridX][GridY] != nullptr) return;

		if (TileTypes.IsValidIndex(CurrentTileIndex))
		{
			ATileBase* SelectedTile = TileTypes[CurrentTileIndex];
			Map[GridX][GridY] = SelectedTile;

			FTransform TileTransform(FRotator(0, CurrentTileYaw, 0), GridLoc + GridOffset);
			SelectedTile->InstancedMesh->AddInstance(
				SelectedTile->InstancedMesh->GetRelativeTransform() * TileTransform,
				true);
		}

		UE_LOG(LogTemp, Warning, TEXT("Hit: %s - %f,%f,%f"),
			HitActor ? *HitActor->GetActorLabel() : TEXT("None"),
			Location.X, Location.Y, Location.Z);
	}
	else if (Input->WasJustPressed(EKeys::MouseScrollDown))
	{
		CurrentTileIndex = (CurrentTileIndex + 1) % TileTypes.Num();
		UE_LOG(LogTemp, Warning, TEXT("TileType: %s"), *TileTypes[CurrentTileIndex]->GetActorLabel());
	}
	else if (Input->WasJustPressed(EKeys::MouseScrollUp))
	{
		CurrentTileIndex = (CurrentTileIndex - 1 + TileTypes.Num()) % TileTypes.Num();
		UE_LOG(LogTemp, Warning, TEXT("TileType: %s"), *TileTypes[CurrentTileIndex]->GetActorLabel());
	}
	else if (Input->WasJustPressed(EKeys::RightMouseButton))
	{
		CurrentTileYaw += 90.0f;
		if (CurrentTileYaw >= 360.0f) CurrentTileYaw = 0.0f;
		UE_LOG(LogTemp, Warning, TEXT("Rotation: %f"), CurrentTileYaw);
	}
	else
	{
		GridSelection->SetWorldLocation(GridLoc + GridOffset);
		if (TileTypes.IsValidIndex(CurrentTileIndex) && TileTypes[CurrentTileIndex]->BaseMesh)
		{
			TilePreview->SetStaticMesh(TileTypes[CurrentTileIndex]->BaseMesh);
			FVector PreviewLoc = GridLoc + GridOffset;
			PreviewLoc.Z += TileTypes[CurrentTileIndex]->InstancedMesh->GetRelativeLocation().Z;
			TilePreview->SetWorldLocation(PreviewLoc);
			TilePreview->SetWorldRotation(FRotator(0, CurrentTileYaw, 0));
		}
	}
}