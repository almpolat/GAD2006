#include "NetBaseCharacter.h"
#include "NetPlayerState.h"
#include "Engine/DataTable.h"

ANetBaseCharacter::ANetBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	PartFace = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PartFace"));
	PartFace->SetupAttachment(GetMesh());

	PartHair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PartHair"));
	PartHair->SetupAttachment(GetMesh());

	PartBeard = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PartBeard"));
	PartBeard->SetupAttachment(GetMesh());

	PartEyes = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PartEyes"));
	PartEyes->SetupAttachment(GetMesh());

	PartHands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PartHands"));
	PartHands->SetupAttachment(GetMesh());

	PartLegs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PartLegs"));
	PartLegs->SetupAttachment(GetMesh());

	PlayerInfoReceived = false;
	MaleDataTable = nullptr;
	FemaleDataTable = nullptr;

	for (int i = 0; i < (int)EBodyPart::BP_COUNT; i++)
		BodyPartIndices[i] = 0;
}

void ANetBaseCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ANetBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() == ENetMode::NM_Standalone) return;
	SetActorHiddenInGame(true);
	CheckPlayerState();
}

void ANetBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANetBaseCharacter::SubmitPlayerInfoToServer_Implementation(FSPlayerInfo Info)
{
	ANetPlayerState* State = GetPlayerState<ANetPlayerState>();
	if (!State) return;
	State->Data.Nickname = Info.Nickname;
	State->Data.CustomizationData = Info.CustomizationData;
	State->Data.TeamID = State->Data.TeamID;
	PlayerInfoReceived = true;
}

void ANetBaseCharacter::CheckPlayerState()
{
	ANetPlayerState* State = GetPlayerState<ANetPlayerState>();
	if (State == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("State == nullptr"));
		GWorld->GetTimerManager().SetTimer(ClientDataCheckTimer, this,
			&ANetBaseCharacter::CheckPlayerState, 0.25f, false);
	}
	else
	{
		if (IsLocallyControlled())
		{
			UNetGameInstance* Instance = Cast<UNetGameInstance>(GWorld->GetGameInstance());
			if (Instance)
			{
				SubmitPlayerInfoToServer(Instance->PlayerInfo);
			}
		}
		CheckPlayerInfo();
	}
}

void ANetBaseCharacter::CheckPlayerInfo()
{
	ANetPlayerState* State = GetPlayerState<ANetPlayerState>();
	if (State && PlayerInfoReceived)
	{
		ParseCustomizationData(State->Data.CustomizationData);
		UpdateBodyParts();
		OnPlayerInfoChanged();
		SetActorHiddenInGame(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("State Not Received!"));
		GWorld->GetTimerManager().SetTimer(ClientDataCheckTimer, this,
			&ANetBaseCharacter::CheckPlayerInfo, 0.25f, false);
	}
}

FString ANetBaseCharacter::GetCustomizationData()
{
	FString Data;
	for (int i = 0; i < (int)EBodyPart::BP_COUNT; i++)
	{
		Data += FString::FromInt(BodyPartIndices[i]);
		if (i < ((int)(EBodyPart::BP_COUNT)-1)) Data += TEXT(",");
	}
	return Data;
}

void ANetBaseCharacter::ParseCustomizationData(FString BodyPartData)
{
	TArray<FString> ArrayData;
	BodyPartData.ParseIntoArray(ArrayData, TEXT(","));
	for (int i = 0; i < ArrayData.Num(); i++)
	{
		BodyPartIndices[i] = FCString::Atoi(*ArrayData[i]);
	}
}

void ANetBaseCharacter::ChangeGender(bool isFemale)
{
	ChangeBodyPart(EBodyPart::BP_BodyType, isFemale ? 1 : 0, true);
	UpdateBodyParts();
}

void ANetBaseCharacter::ChangeBodyPart(EBodyPart index, int value, bool DirectSet)
{
	FSMeshAssetList* List = GetBodyPartList(index, BodyPartIndices[(int)EBodyPart::BP_BodyType] != 0);
	if (List == nullptr) return;

	int CurrentIndex = BodyPartIndices[(int)index];

	if (DirectSet)
		CurrentIndex = value;
	else
		CurrentIndex += value;

	int Num = List->ListSkeletal.Num() + List->ListStatic.Num();
	if (Num == 0) return;

	if (CurrentIndex < 0)
		CurrentIndex += Num;
	else
		CurrentIndex %= Num;

	BodyPartIndices[(int)index] = CurrentIndex;

	switch (index)
	{
	case EBodyPart::BP_Face:
		if (PartFace && List->ListSkeletal.IsValidIndex(CurrentIndex))
			PartFace->SetSkeletalMeshAsset(List->ListSkeletal[CurrentIndex]);
		break;
	case EBodyPart::BP_Hair:
		if (PartHair && List->ListStatic.IsValidIndex(CurrentIndex))
			PartHair->SetStaticMesh(List->ListStatic[CurrentIndex]);
		break;
	case EBodyPart::BP_Chest:
		if (GetMesh() && List->ListSkeletal.IsValidIndex(CurrentIndex))
			GetMesh()->SetSkeletalMeshAsset(List->ListSkeletal[CurrentIndex]);
		break;
	case EBodyPart::BP_Hands:
		if (PartHands && List->ListSkeletal.IsValidIndex(CurrentIndex))
			PartHands->SetSkeletalMeshAsset(List->ListSkeletal[CurrentIndex]);
		break;
	case EBodyPart::BP_Legs:
		if (PartLegs && List->ListSkeletal.IsValidIndex(CurrentIndex))
			PartLegs->SetSkeletalMeshAsset(List->ListSkeletal[CurrentIndex]);
		break;
	case EBodyPart::BP_Beard:
		if (PartBeard && List->ListStatic.IsValidIndex(CurrentIndex))
			PartBeard->SetStaticMesh(List->ListStatic[CurrentIndex]);
		break;
	default: break;
	}
}

void ANetBaseCharacter::UpdateBodyParts()
{
	ChangeBodyPart(EBodyPart::BP_Face, 0, false);
	ChangeBodyPart(EBodyPart::BP_Beard, 0, false);
	ChangeBodyPart(EBodyPart::BP_Chest, 0, false);
	ChangeBodyPart(EBodyPart::BP_Hair, 0, false);
	ChangeBodyPart(EBodyPart::BP_Hands, 0, false);
	ChangeBodyPart(EBodyPart::BP_Legs, 0, false);
}

FSMeshAssetList* ANetBaseCharacter::GetBodyPartList(EBodyPart part, bool isFemale)
{
	UDataTable* Table = isFemale ? FemaleDataTable : MaleDataTable;
	if (!Table) return nullptr;

	FName RowName;
	switch (part)
	{
	case EBodyPart::BP_Face:    RowName = FName("Face");  break;
	case EBodyPart::BP_Hair:    RowName = FName("Hair");  break;
	case EBodyPart::BP_Chest:   RowName = FName("Chest"); break;
	case EBodyPart::BP_Hands:   RowName = FName("Hands"); break;
	case EBodyPart::BP_Legs:    RowName = FName("Legs");  break;
	case EBodyPart::BP_Beard:   RowName = FName("Beard"); break;
	default: return nullptr;
	}

	return Table->FindRow<FSMeshAssetList>(RowName, "");
}