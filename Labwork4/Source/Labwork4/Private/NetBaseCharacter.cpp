#include "NetBaseCharacter.h"
#include "NetPlayerState.h"
#include "NetGameInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

UDataTable* ANetBaseCharacter::SBodyParts = nullptr;
TArray<FString> ANetBaseCharacter::BodyPartNames = {
    "Face", "Hair", "Beard", "Eyebrows", "Chest", "Hands", "Legs", "BodyType"
};

ANetBaseCharacter::ANetBaseCharacter() : PlayerInfoReceived(false)
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    FMemory::Memset(BodyPartIndices, 0, sizeof(BodyPartIndices));

    static ConstructorHelpers::FObjectFinder<UDataTable> BodyPartsTable(
        TEXT("/Game/Blueprints/DT_BodyParts"));
    if (BodyPartsTable.Succeeded())
        SBodyParts = BodyPartsTable.Object;

    // PartFace önce oluşturulmalı — Hair/Beard/Eyebrows/Eyes ona attach olacak
    PartFace = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PartFace"));
    PartFace->SetupAttachment(GetMesh());
    PartFace->SetLeaderPoseComponent(GetMesh());

    // ── FIX: Ana mesh "head" bone'u yerine PartFace'in headSocket'ine attach ──
    PartHair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PartHair"));
    PartHair->SetupAttachment(PartFace, FName("headSocket"));

    PartBeard = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PartBeard"));
    PartBeard->SetupAttachment(PartFace, FName("headSocket"));

    PartEyebrows = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PartEyebrows"));
    PartEyebrows->SetupAttachment(PartFace, FName("headSocket"));

    PartEyes = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PartEyes"));
    PartEyes->SetupAttachment(PartFace, FName("headSocket"));
    // ─────────────────────────────────────────────────────────────────────────

    PartHands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PartHands"));
    PartHands->SetupAttachment(GetMesh());
    PartHands->SetLeaderPoseComponent(GetMesh());

    PartLegs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PartLegs"));
    PartLegs->SetupAttachment(GetMesh());
    PartLegs->SetLeaderPoseComponent(GetMesh());
}

void ANetBaseCharacter::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ANetBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    // ── FIX: BeginPlay'de head attachment YOK — constructor'da halloldu ──
    // PartHair, PartBeard, PartEyebrows, PartEyes artık PartFace->headSocket'e
    // constructor'da attach ediliyor. Burada tekrar attach etmeye gerek yok.

    if (GetNetMode() == ENetMode::NM_Standalone) return;

    SetActorHiddenInGame(true);
    CheckPlayerState();
}

void ANetBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ANetBaseCharacter::OnConstruction(const FTransform& Transform)
{
    UpdateBodyParts();
}

void ANetBaseCharacter::CheckPlayerState()
{
    ANetPlayerState* State = GetPlayerState<ANetPlayerState>();
    if (State == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("CheckPlayerState: State == nullptr, retrying..."));
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
        UE_LOG(LogTemp, Warning, TEXT("CheckPlayerInfo: State Not Received, retrying..."));
        GWorld->GetTimerManager().SetTimer(ClientDataCheckTimer, this,
            &ANetBaseCharacter::CheckPlayerInfo, 0.25f, false);
    }
}

void ANetBaseCharacter::SubmitPlayerInfoToServer_Implementation(FSPlayerInfo Info)
{
    ANetPlayerState* State = GetPlayerState<ANetPlayerState>();
    if (!State) return;
    State->Data.Nickname = Info.Nickname;
    State->Data.CustomizationData = Info.CustomizationData;
    State->Data.TeamID = State->TeamID;
    PlayerInfoReceived = true;
}

FString ANetBaseCharacter::GetCustomizationData()
{
    FString Data;
    for (size_t i = 0; i < (int)EBodyPart::BP_COUNT; i++)
    {
        Data += FString::FromInt(BodyPartIndices[i]);
        if (i < ((int)EBodyPart::BP_COUNT) - 1) Data += TEXT(",");
    }
    return Data;
}

void ANetBaseCharacter::ParseCustomizationData(FString BodyPartData)
{
    TArray<FString> ArrayData;
    BodyPartData.ParseIntoArray(ArrayData, TEXT(","));
    for (size_t i = 0; i < ArrayData.Num(); i++)
    {
        BodyPartIndices[i] = FCString::Atoi(*ArrayData[i]);
    }
}

void ANetBaseCharacter::ChangeBodyPart(EBodyPart index, int value, bool DirectSet)
{
    FSMeshAssetList* List = GetBodyPartList(index,
        BodyPartIndices[(int)EBodyPart::BP_BodyType] != 0);
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
        if (List->ListSkeletal.IsValidIndex(CurrentIndex))
            PartFace->SetSkeletalMeshAsset(List->ListSkeletal[CurrentIndex]);
        break;
    case EBodyPart::BP_Hair:
        if (List->ListStatic.IsValidIndex(CurrentIndex))
            PartHair->SetStaticMesh(List->ListStatic[CurrentIndex]);
        break;
    case EBodyPart::BP_Beard:
        if (List->ListStatic.IsValidIndex(CurrentIndex))
            PartBeard->SetStaticMesh(List->ListStatic[CurrentIndex]);
        break;
    case EBodyPart::BP_Eyebrows:
        if (List->ListStatic.IsValidIndex(CurrentIndex))
            PartEyebrows->SetStaticMesh(List->ListStatic[CurrentIndex]);
        break;
    case EBodyPart::BP_Chest:
        if (List->ListSkeletal.IsValidIndex(CurrentIndex))
            GetMesh()->SetSkeletalMeshAsset(List->ListSkeletal[CurrentIndex]);
        break;
    case EBodyPart::BP_Hands:
        if (List->ListSkeletal.IsValidIndex(CurrentIndex))
            PartHands->SetSkeletalMeshAsset(List->ListSkeletal[CurrentIndex]);
        break;
    case EBodyPart::BP_Legs:
        if (List->ListSkeletal.IsValidIndex(CurrentIndex))
            PartLegs->SetSkeletalMeshAsset(List->ListSkeletal[CurrentIndex]);
        break;
    default:
        break;
    }
}

void ANetBaseCharacter::ChangeGender(bool _isFemale)
{
    BodyPartIndices[(int)EBodyPart::BP_BodyType] = _isFemale ? 1 : 0;
    UpdateBodyParts();
}

void ANetBaseCharacter::UpdateBodyParts()
{
    ChangeBodyPart(EBodyPart::BP_Face, 0, false);
    ChangeBodyPart(EBodyPart::BP_Beard, 0, false);
    ChangeBodyPart(EBodyPart::BP_Eyebrows, 0, false);
    ChangeBodyPart(EBodyPart::BP_Chest, 0, false);
    ChangeBodyPart(EBodyPart::BP_Hair, 0, false);
    ChangeBodyPart(EBodyPart::BP_Hands, 0, false);
    ChangeBodyPart(EBodyPart::BP_Legs, 0, false);
}

FSMeshAssetList* ANetBaseCharacter::GetBodyPartList(EBodyPart part, bool isFemale)
{
    FString Name = FString::Printf(TEXT("%s%s"),
        isFemale ? TEXT("Female") : TEXT("Male"),
        *BodyPartNames[(int)part]);
    return SBodyParts ? SBodyParts->FindRow<FSMeshAssetList>(*Name, nullptr) : nullptr;
}