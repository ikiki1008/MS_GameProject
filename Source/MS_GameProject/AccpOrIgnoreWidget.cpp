#include "AccpOrIgnoreWidget.h"
#include "Components/EditableText.h"
#include "Styling/SlateTypes.h"
#include "Components/Button.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MsPlayer.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogAccpOrIgnoreWidget);

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
            Player->SetWidgetActive(true); //�÷��̾� Ŭ������ ������ Ȱ��ȭ �Ǿ��ٴ� ���� ������
            Player->ChooseWidget = this;
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
        Player->ChooseWidget = nullptr; //�÷��̾� Ŭ������ ������ �����ٴ� ���� ������
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
            IgnoreBtn->OnClicked.Broadcast(); // Ignore ��ư Ŭ�� ó��
            Result = TEXT("Ignore");
            UE_LOG(LogAccpOrIgnoreWidget, Warning, TEXT("ignore btn clicked"));
        }
        else if (BegBtn && BegBtn->HasKeyboardFocus()) {
            BegBtn->OnClicked.Broadcast(); // Beg ��ư Ŭ�� ó��
            Result = TEXT("Begging");
            UE_LOG(LogAccpOrIgnoreWidget, Warning, TEXT("begging btn clicked"));
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