#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ASEnemyHealthBarWidget.generated.h"

UCLASS()
class ARENASURVIVORS_API UASEnemyHealthBarWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* ProgressBar_HP;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetHealthPercent(float Percent);
};