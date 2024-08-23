#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/MultiLineEditableTextBox.h"
#include "BlackSmithWidget.generated.h"

UCLASS()
class MS_GAMEPROJECT_API UBlackSmithWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override; //cpp 파일에서 제공하는 위젯 블루프린트 부르기
	
protected:
	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableTextBox* textBox; //textbox 변수 바인딩
};
