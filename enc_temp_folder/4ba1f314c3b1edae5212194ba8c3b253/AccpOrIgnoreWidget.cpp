#include "AccpOrIgnoreWidget.h"
#include "Components/EditableText.h"
#include "Styling/SlateTypes.h"
#include "Components/Button.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MsPlayer.h"
#include "Kismet/GameplayStatics.h"

void UAccpOrIgnoreWidget::NativeConstruct() {
    Super::NativeConstruct();

    if (TextBox) {
        // ù��° �ؽ�Ʈ ����
        TextBox->SetText(FText::FromString("\nHe's asking for 500 bucks at least\nBut you ain't got stacks...\nWhat would you do? You can just ignore and keep adventuring"));
        TextBox->SetForegroundColor(FLinearColor::White);

        // �ؽ�Ʈ ��Ÿ�� ����
        FTextBlockStyle TextStyle = TextBox->WidgetStyle.TextStyle;
        TextStyle.SetColorAndOpacity(FSlateColor(FLinearColor::White));  // �ؽ�Ʈ ������ ������� ����
        TextStyle.SetShadowOffset(FVector2D(1.0f, 1.0f));  // �׸��� ������ ����
        TextStyle.SetShadowColorAndOpacity(FLinearColor(0, 0, 0, 0.5f));  // �׸��� ���� ���� ����

        TextBox->SetTextStyle(TextStyle);

        // ����� ������ �����ϰ� ����
        FSlateBrush TransparentBrush;
        TransparentBrush.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0.8f));  // ������ ������ ���

        TextBox->WidgetStyle.SetBackgroundImageNormal(TransparentBrush);
        TextBox->WidgetStyle.SetBackgroundImageHovered(TransparentBrush);
        TextBox->WidgetStyle.SetBackgroundImageFocused(TransparentBrush);

        AMsPlayer* Player = Cast<AMsPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
        if (Player) {
            Player->SetWidgetActive(true);
            Player->ChooseWidget = this;
        }
    }

    if (IgnoreBtn) {
        SetButtonFocus(IgnoreBtn);
    }
    else {
        SetButtonFocus(BegBtn);
    }

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAccpOrIgnoreWidget::CheckResult, 1.0f, true);

    //UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer(), this, EMouseLockMode::DoNotLock, false);
   /* UWidgetBlueprintLibrary::SetFocusToGameViewport();*/
}

// Result ������ üũ�ϴ� �Լ�
void UAccpOrIgnoreWidget::CheckResult() {
    if (Result.IsEmpty()) {
        UE_LOG(LogTemp, Warning, TEXT("Result is null"));
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Result: %s"), *Result);
    }
}

void UAccpOrIgnoreWidget::NativeDestruct(){
    Super::NativeDestruct();

    AMsPlayer* Player = Cast<AMsPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
    if (Player){
        Player->SetWidgetActive(false); // Set inactive when the widget is destroyed
        Player->ChooseWidget = nullptr; // Clear the widget instance
    }
}

void UAccpOrIgnoreWidget::SetButtonFocus(UButton* Button) {
    if (Button) {
        Button->SetKeyboardFocus();
    }
}

FReply UAccpOrIgnoreWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) {
    FKey KeyPressed = InKeyEvent.GetKey();
    
    if (KeyPressed == EKeys::A) {
        NavigateLeft();
        return FReply::Handled();
    }
    else if (KeyPressed == EKeys::D) {
        NavigateRight();
        return FReply::Handled();
    }
    else if (KeyPressed == EKeys::E) {
        if (IgnoreBtn && IgnoreBtn->HasKeyboardFocus()) {
            IgnoreBtn->OnPressed.Broadcast(); // Ignore ��ư Ŭ�� ó��
        }
        else if (BegBtn && BegBtn->HasKeyboardFocus()) {
            BegBtn->OnPressed.Broadcast(); // Beg ��ư Ŭ�� ó��
        }
        return FReply::Handled();
    }

    return Super::NativeOnKeyDown(InGeometry, InKeyEvent); 
}

void UAccpOrIgnoreWidget::NavigateLeft() {
    if (IgnoreBtn && IgnoreBtn->HasKeyboardFocus()) {
        IgnoreBtn->SetColorAndOpacity(FLinearColor::White); // ���� ��ư ���� ����
        SetButtonFocus(BegBtn);
        BegBtn->SetColorAndOpacity(FLinearColor::Gray); // ��Ŀ���� ��ư ���� ����
    }
    else if (BegBtn && BegBtn->HasKeyboardFocus()) {
        BegBtn->SetColorAndOpacity(FLinearColor::White); // ���� ��ư ���� ����
        SetButtonFocus(IgnoreBtn);
        IgnoreBtn->SetColorAndOpacity(FLinearColor::Gray); // ��Ŀ���� ��ư ���� ����
    }
}

void UAccpOrIgnoreWidget::NavigateRight() {
    if (IgnoreBtn && IgnoreBtn->HasKeyboardFocus()) {
        IgnoreBtn->SetColorAndOpacity(FLinearColor::White); // ���� ��ư ���� ����
        SetButtonFocus(BegBtn);
        BegBtn->SetColorAndOpacity(FLinearColor::Gray); // ��Ŀ���� ��ư ���� ����
    }
    else if (BegBtn && BegBtn->HasKeyboardFocus()) {
        BegBtn->SetColorAndOpacity(FLinearColor::White); // ���� ��ư ���� ����
        SetButtonFocus(IgnoreBtn);
        IgnoreBtn->SetColorAndOpacity(FLinearColor::Gray); // ��Ŀ���� ��ư ���� ����
    }
}