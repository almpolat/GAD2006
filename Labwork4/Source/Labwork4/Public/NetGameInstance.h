#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NetGameInstance.generated.h"

UENUM(BlueprintType)
enum class EPlayerTeam : uint8
{
    TEAM_Unknown  UMETA(DisplayName = "Unknown"),
    TEAM_Blue     UMETA(DisplayName = "Blue"),
    TEAM_Red      UMETA(DisplayName = "Red")
};

UENUM(BlueprintType)
enum class EGameResult : uint8
{
    RESULT_Unknown  UMETA(DisplayName = "Unknown"),
    RESULT_Won      UMETA(DisplayName = "Won"),
    RESULT_Lost     UMETA(DisplayName = "Lost")
};

USTRUCT(BlueprintType)
struct FSPlayerInfo
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Nickname;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CustomizationData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPlayerTeam TeamID;
};

UCLASS()
class LABWORK4_API UNetGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void Host(FString MapName, const FSPlayerInfo& Info);

    UFUNCTION(BlueprintCallable)
    void Join(FString Address, const FSPlayerInfo& Info);

    UPROPERTY(BlueprintReadWrite)
    FSPlayerInfo PlayerInfo;
};