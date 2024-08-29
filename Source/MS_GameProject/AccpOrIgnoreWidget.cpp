#include "AccpOrIgnoreWidget.h"
#include "Components/EditableText.h"
#include "Styling/SlateTypes.h"
#include "Components/Button.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MsPlayer.h"
#include "MsBlackSmith.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogAccpOrIgnoreWidget);

void UAccpOrIgnoreWidget::NativeConstruct() {
    Super::NativeConstruct();

    Result = "";

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
            Player->SetWidgetActive(true); //플레이어 클래스로 위젯이 활성화 되었다는 값을 보낸다
            Player->ChooseWidget = this;
        }

        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMsBlackSmith::StaticClass(), FoundActors);

        if (FoundActors.Num() > 0) {
            AMsBlackSmith* BlackSmith = Cast<AMsBlackSmith>(FoundActors[0]);
            if (BlackSmith) {
                BlackSmith->IsChooseWidgetActive(true);
                BlackSmith->AccpOrIgnoreWidget = this;
            }
        }
    }

    if (IgnoreBtn) {
        SetButtonFocus(IgnoreBtn);
    }
    else {
        SetButtonFocus(BegBtn);
    }
}

void UAccpOrIgnoreWidget::NativeDestruct(){
    Super::NativeDestruct();

    AMsPlayer* Player = Cast<AMsPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
    if (Player){
        Player->SetWidgetActive(false); 
        Player->ChooseWidget = nullptr; //플레이어 클래스로 위젯이 닫혔다는 값을 보낸다
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
            Result = "Ignore";
            IgnoreBtn->OnClicked.Broadcast(); // Ignore 버튼 클릭 처리
            UE_LOG(LogAccpOrIgnoreWidget, Warning, TEXT("ignore btn clicked"));
        }
        else if (BegBtn && BegBtn->HasKeyboardFocus()) {
            Result = "Begging";
            BegBtn->OnClicked.Broadcast(); // Beg 버튼 클릭 처리
            UE_LOG(LogAccpOrIgnoreWidget, Warning, TEXT("begging btn clicked"));
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