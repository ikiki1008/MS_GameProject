#include "BlackSmithWidget.h"
#include "Components/MultiLineEditableTextBox.h"

void UBlackSmithWidget::NativeConstruct() {
    Super::NativeConstruct();

    if (textBox) {
        // 첫번째 텍스트 설정
        //textBox->SetText(FText::FromString("\nThere is a guy looks weird but has dirty sexy vibe.. \n He's trying to introduce himself.\n Now let's listen to what he says."));
        textBox->SetText(FText::FromString("\nOops, He's asking for 100 bucks but you ain't got money now \n How you gonna do now? \n"));

        textBox->SetForegroundColor(FLinearColor::White);

        // 텍스트 스타일 설정
        FTextBlockStyle TextStyle = textBox->WidgetStyle.TextStyle;
        TextStyle.SetColorAndOpacity(FSlateColor(FLinearColor::White));  // 텍스트 색상을 흰색으로 설정
        TextStyle.SetShadowOffset(FVector2D(1.0f, 1.0f));  // 그림자 오프셋 설정
        TextStyle.SetShadowColorAndOpacity(FLinearColor(0, 0, 0, 0.5f));  // 그림자 색과 투명도 설정

        textBox->SetTextStyle(TextStyle);

        // 배경을 완전히 투명하게 설정
        FSlateBrush TransparentBrush;
        TransparentBrush.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0.6f));  // 완전히 투명한 배경

        textBox->WidgetStyle.SetBackgroundImageNormal(TransparentBrush);
        textBox->WidgetStyle.SetBackgroundImageHovered(TransparentBrush);
        textBox->WidgetStyle.SetBackgroundImageFocused(TransparentBrush);
    }
}
