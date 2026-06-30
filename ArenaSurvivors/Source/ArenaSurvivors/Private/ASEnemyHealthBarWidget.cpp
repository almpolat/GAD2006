#include "ASEnemyHealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Engine/Engine.h"

void UASEnemyHealthBarWidget::SetHealthPercent(float Percent)
{
    float Clamped = FMath::Clamp(Percent, 0.f, 1.f);

    if (ProgressBar_HP)
    {
        ProgressBar_HP->SetPercent(Clamped);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SetHealthPercent: ProgressBar_HP is NULL!"));
    }
}