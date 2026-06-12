#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"
#include "NetGameInstance.h"
#include "NetBaseCharacter.generated.h"

UENUM(BlueprintType)
enum class EBodyPart : uint8
{
    BP_Face      UMETA(DisplayName = "Face"),
    BP_Hair      UMETA(DisplayName = "Hair"),
    BP_Beard     UMETA(DisplayName = "Beard"),
    BP_Eyebrows  UMETA(DisplayName = "Eyebrows"),
    BP_Chest     UMETA(DisplayName = "Chest"),
    BP_Hands     UMETA(DisplayName = "Hands"),
    BP_Legs      UMETA(DisplayName = "Legs"),
    BP_BodyType  UMETA(DisplayName = "BodyType"),
    BP_COUNT     UMETA(DisplayName = "COUNT")
};

USTRUCT(BlueprintType)
struct FSMeshAssetList : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<USkeletalMesh*> ListSkeletal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UStaticMesh*> ListStatic;
};

UCLASS()
class LABWORK4_API ANetBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ANetBaseCharacter();

    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void Tick(float DeltaTime) override;
    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintPure)
    FString GetCustomizationData();

    void ParseCustomizationData(FString Data);

    UFUNCTION(BlueprintCallable)
    void ChangeBodyPart(EBodyPart index, int value, bool DirectSet);

    UFUNCTION(BlueprintCallable)
    void ChangeGender(bool isFemale);

    UFUNCTION(Server, Reliable)
    void SubmitPlayerInfoToServer(FSPlayerInfo Info);

    UFUNCTION(BlueprintImplementableEvent)
    void OnPlayerInfoChanged();

    UFUNCTION()
    void CheckPlayerState();

    UFUNCTION()
    void CheckPlayerInfo();

    void UpdateBodyParts();

    static FSMeshAssetList* GetBodyPartList(EBodyPart part, bool isFemale);

    bool PlayerInfoReceived;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USkeletalMeshComponent* PartFace;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* PartHair;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* PartBeard;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* PartEyebrows;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* PartEyes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USkeletalMeshComponent* PartHands;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USkeletalMeshComponent* PartLegs;

    int BodyPartIndices[(int)EBodyPart::BP_COUNT];

private:
    FTimerHandle ClientDataCheckTimer;

    static UDataTable* SBodyParts;
    static TArray<FString> BodyPartNames;
};