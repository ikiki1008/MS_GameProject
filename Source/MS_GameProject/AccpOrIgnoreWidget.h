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
	virtual void NativeConstruct() override; //cpp 파일에서 제공하는 위젯 블루프린트 부르기
	virtual void NativeDestruct() override;

	void NavigateLeft();
	void NavigateRight();

	UPROPERTY(BlueprintReadWrite, Category = "Widget")
	FString Result; // 버튼 클릭 시 결과 string 을 담을 버튼

protected:
	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableTextBox* TextBox; // Editable Text Multi Line 변수 바인딩

	UPROPERTY(meta = (BindWidget))
	UButton* IgnoreBtn;  // 버튼 바인딩

	UPROPERTY(meta = (BindWidget))
	UButton* BegBtn;  // 버튼 바인딩

private:
	// Functions to handle button focus and input
	void SetButtonFocus(UButton* Button);

	// Input handling functions
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};

DECLARE_LOG_CATEGORY_EXTERN(LogAccpOrIgnoreWidget, Log, All);
