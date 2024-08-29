#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/Button.h"
#include "AccpOrIgnoreWidget.generated.h"

UCLASS()
class MS_GAMEPROJECT_API UAccpOrIgnoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override; //cpp ���Ͽ��� �����ϴ� ���� �������Ʈ �θ���
	virtual void NativeDestruct() override;

	void NavigateLeft();
	void NavigateRight();

	UPROPERTY(BlueprintReadWrite, Category = "Widget")
	FString Result; // Result ���� ����

protected:
	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableTextBox* TextBox; // Editable Text Multi Line ���� ���ε�

	UPROPERTY(meta = (BindWidget))
	UButton* IgnoreBtn;  // Binding for the Ignore button

	UPROPERTY(meta = (BindWidget))
	UButton* BegBtn;  // Binding for the Beg button

private:
	// Functions to handle button focus and input
	void SetButtonFocus(UButton* Button);
	void SimulateButtonClick();
	void CheckResult();

	// TimerHandle ����
	FTimerHandle TimerHandle;

	// Input handling functions
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
