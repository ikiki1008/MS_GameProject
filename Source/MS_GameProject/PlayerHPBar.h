#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHPBar.generated.h"

UCLASS()
class MS_GAMEPROJECT_API UPlayerHPBar : public UUserWidget
{
    GENERATED_BODY()

public:
    UPlayerHPBar(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "HP")
    void UpdateHP(float Life);

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Stats")
    float MaxHP;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Stats")
    float CurrentHP;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HPProgressBar;

    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintPure, Category = "UI")
    float GetHPPercent() const;
};


DECLARE_LOG_CATEGORY_EXTERN(LogPlayerHPBar, Log, All);
