#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ASPlayerNameTagWidget.generated.h"

UCLASS()
class ARENASURVIVORS_API UASPlayerNameTagWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TextBlock_Nickname;

    UFUNCTION(BlueprintCallable, Category = "Player")
    void SetNickname(const FString& Nickname);
};