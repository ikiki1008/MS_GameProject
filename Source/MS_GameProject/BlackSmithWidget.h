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
	virtual void NativeConstruct() override; //cpp ���Ͽ��� �����ϴ� ���� �������Ʈ �θ���
	
protected:
	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableTextBox* textBox; //textbox ���� ���ε�
};
