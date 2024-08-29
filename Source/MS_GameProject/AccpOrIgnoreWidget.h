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
	FString Result; // ��ư Ŭ�� �� ��� string �� ���� ��ư

protected:
	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableTextBox* TextBox; // Editable Text Multi Line ���� ���ε�

	UPROPERTY(meta = (BindWidget))
	UButton* IgnoreBtn;  // ��ư ���ε�

	UPROPERTY(meta = (BindWidget))
	UButton* BegBtn;  // ��ư ���ε�

private:
	// Functions to handle button focus and input
	void SetButtonFocus(UButton* Button);

	// Input handling functions
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};

DECLARE_LOG_CATEGORY_EXTERN(LogAccpOrIgnoreWidget, Log, All);
