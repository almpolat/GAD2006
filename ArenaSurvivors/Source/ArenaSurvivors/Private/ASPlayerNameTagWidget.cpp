#include "ASPlayerNameTagWidget.h"
#include "Components/TextBlock.h"

void UASPlayerNameTagWidget::SetNickname(const FString& Nickname)
{
    if (TextBlock_Nickname)
    {
        FString DisplayName = Nickname.IsEmpty() ? TEXT("Player") : Nickname;
        TextBlock_Nickname->SetText(FText::FromString(DisplayName));
    }
}