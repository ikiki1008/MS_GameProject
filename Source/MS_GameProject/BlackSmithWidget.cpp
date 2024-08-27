#include "BlackSmithWidget.h"
#include "Components/MultiLineEditableTextBox.h"

void UBlackSmithWidget::NativeConstruct() {
    Super::NativeConstruct();

    if (MultiLine) {
        // ù��° �ؽ�Ʈ ����
        //textBox->SetText(FText::FromString("\nThere is a guy looks weird but has dirty sexy vibe.. \n He's trying to introduce himself.\n Now let's listen to what he says."));
        MultiLine->SetForegroundColor(FLinearColor::White);

        // �ؽ�Ʈ ��Ÿ�� ����
        FTextBlockStyle TextStyle = MultiLine->WidgetStyle.TextStyle;
        TextStyle.SetColorAndOpacity(FSlateColor(FLinearColor::White));  // �ؽ�Ʈ ������ ������� ����
        TextStyle.SetShadowOffset(FVector2D(1.0f, 1.0f));  // �׸��� ������ ����
        TextStyle.SetShadowColorAndOpacity(FLinearColor(0, 0, 0, 0.5f));  // �׸��� ���� ���� ����

        MultiLine->SetTextStyle(TextStyle);

        // ����� ������ �����ϰ� ����
        FSlateBrush TransparentBrush;
        TransparentBrush.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0.6f));  // ������ ������ ���

        MultiLine->WidgetStyle.SetBackgroundImageNormal(TransparentBrush);
        MultiLine->WidgetStyle.SetBackgroundImageHovered(TransparentBrush);
        MultiLine->WidgetStyle.SetBackgroundImageFocused(TransparentBrush);
    }
}
