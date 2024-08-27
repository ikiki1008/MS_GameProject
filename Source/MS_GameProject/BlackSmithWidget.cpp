#include "BlackSmithWidget.h"
#include "Components/MultiLineEditableTextBox.h"

void UBlackSmithWidget::NativeConstruct() {
    Super::NativeConstruct();

    if (MultiLine) {
        // 첫번째 텍스트 설정
        //textBox->SetText(FText::FromString("\nThere is a guy looks weird but has dirty sexy vibe.. \n He's trying to introduce himself.\n Now let's listen to what he says."));
        MultiLine->SetForegroundColor(FLinearColor::White);

        // 텍스트 스타일 설정
        FTextBlockStyle TextStyle = MultiLine->WidgetStyle.TextStyle;
        TextStyle.SetColorAndOpacity(FSlateColor(FLinearColor::White));  // 텍스트 색상을 흰색으로 설정
        TextStyle.SetShadowOffset(FVector2D(1.0f, 1.0f));  // 그림자 오프셋 설정
        TextStyle.SetShadowColorAndOpacity(FLinearColor(0, 0, 0, 0.5f));  // 그림자 색과 투명도 설정

        MultiLine->SetTextStyle(TextStyle);

        // 배경을 완전히 투명하게 설정
        FSlateBrush TransparentBrush;
        TransparentBrush.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0.6f));  // 완전히 투명한 배경

        MultiLine->WidgetStyle.SetBackgroundImageNormal(TransparentBrush);
        MultiLine->WidgetStyle.SetBackgroundImageHovered(TransparentBrush);
        MultiLine->WidgetStyle.SetBackgroundImageFocused(TransparentBrush);
    }
}
