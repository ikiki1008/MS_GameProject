#include "PlayerHPBar.h"
#include "Components/ProgressBar.h"

DEFINE_LOG_CATEGORY(LogPlayerHPBar);

UPlayerHPBar::UPlayerHPBar(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    CurrentHP = 1000.0f;
    MaxHP = 1000.0f;
}

void UPlayerHPBar::NativeConstruct()
{
    Super::NativeConstruct();
    UpdateHP(CurrentHP);
}

void UPlayerHPBar::UpdateHP(float Life) {
    CurrentHP = Life;
    if (HPProgressBar)
    {
        HPProgressBar->SetPercent(GetHPPercent());
    }
    UE_LOG(LogPlayerHPBar, Warning, TEXT(" $$$$ Current Player's HP: %f"), CurrentHP);
}

float UPlayerHPBar::GetHPPercent() const {
    return MaxHP > 0.0f ? CurrentHP / MaxHP : 0.0f;
}
