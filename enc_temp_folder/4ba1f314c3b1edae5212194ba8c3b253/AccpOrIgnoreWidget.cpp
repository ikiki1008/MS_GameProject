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
        // 첫번째 텍스트 설정
        TextBox->SetText(FText::FromString("\nHe's asking for 500 bucks at least\nBut you ain't got stacks...\nWhat would you do? You can just ignore and keep adventuring"));
        TextBox->SetForegroundColor(FLinearColor::White);

        // 텍스트 스타일 설정
        FTextBlockStyle TextStyle = TextBox->WidgetStyle.TextStyle;
        TextStyle.SetColorAndOpacity(FSlateColor(FLinearColor::White));  // 텍스트 색상을 흰색으로 설정
        TextStyle.SetShadowOffset(FVector2D(1.0f, 1.0f));  // 그림자 오프셋 설정
        TextStyle.SetShadowColorAndOpacity(FLinearColor(0, 0, 0, 0.5f));  // 그림자 색과 투명도 설정

        TextBox->SetTextStyle(TextStyle);

        // 배경을 완전히 투명하게 설정
        FSlateBrush TransparentBrush;
        TransparentBrush.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0.8f));  // 완전히 투명한 배경

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

// Result 변수를 체크하는 함수
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
            IgnoreBtn->OnPressed.Broadcast(); // Ignore 버튼 클릭 처리
        }
        else if (BegBtn && BegBtn->HasKeyboardFocus()) {
            BegBtn->OnPressed.Broadcast(); // Beg 버튼 클릭 처리
        }
        return FReply::Handled();
    }

    return Super::NativeOnKeyDown(InGeometry, InKeyEvent); 
}

void UAccpOrIgnoreWidget::NavigateLeft() {
    if (IgnoreBtn && IgnoreBtn->HasKeyboardFocus()) {
        IgnoreBtn->SetColorAndOpacity(FLinearColor::White); // 기존 버튼 색상 복원
        SetButtonFocus(BegBtn);
        BegBtn->SetColorAndOpacity(FLinearColor::Gray); // 포커스된 버튼 색상 변경
    }
    else if (BegBtn && BegBtn->HasKeyboardFocus()) {
        BegBtn->SetColorAndOpacity(FLinearColor::White); // 기존 버튼 색상 복원
        SetButtonFocus(IgnoreBtn);
        IgnoreBtn->SetColorAndOpacity(FLinearColor::Gray); // 포커스된 버튼 색상 변경
    }
}

void UAccpOrIgnoreWidget::NavigateRight() {
    if (IgnoreBtn && IgnoreBtn->HasKeyboardFocus()) {
        IgnoreBtn->SetColorAndOpacity(FLinearColor::White); // 기존 버튼 색상 복원
        SetButtonFocus(BegBtn);
        BegBtn->SetColorAndOpacity(FLinearColor::Gray); // 포커스된 버튼 색상 변경
    }
    else if (BegBtn && BegBtn->HasKeyboardFocus()) {
        BegBtn->SetColorAndOpacity(FLinearColor::White); // 기존 버튼 색상 복원
        SetButtonFocus(IgnoreBtn);
        IgnoreBtn->SetColorAndOpacity(FLinearColor::Gray); // 포커스된 버튼 색상 변경
    }
}