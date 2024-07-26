// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHPBar.generated.h"

/**
 * 
 */
UCLASS()
class MS_GAMEPROJECT_API UPlayerHPBar : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void UpdateHP(float CurrentHP, float MaxHP);
};
