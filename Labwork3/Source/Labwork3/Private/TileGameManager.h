#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerInput.h"
#include "TileBase.h"
#include "TileGameManager.generated.h"

#define MAX_MAP_GRID_SIZE 100

UCLASS()
class LABWORK3_API ATileGameManager : public AActor
{
	GENERATED_BODY()

public:
	ATileGameManager();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GridSelection;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TilePreview;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void OnActorInteraction(AActor* Actor, FVector& Location, bool IsPressed);

	UPROPERTY(EditAnywhere)
	int GridSize;

	UPROPERTY(EditAnywhere)
	FVector GridOffset;

	UPROPERTY(EditAnywhere)
	TArray<ATileBase*> TileTypes;

	UPROPERTY(EditAnywhere)
	int MapExtendsInGrids;

	int CurrentTileIndex;
	float CurrentTileYaw;

	ATileBase* Map[MAX_MAP_GRID_SIZE][MAX_MAP_GRID_SIZE];
};