#include "BlackSmithWidget.h"
#include "Components/MultiLineEditableTextBox.h"

void UBlackSmithWidget::NativeConstruct() {
    Super::NativeConstruct();

    if (textBox) {
        // ù��° �ؽ�Ʈ ����
        //textBox->SetText(FText::FromString("\nThere is a guy looks weird but has dirty sexy vibe.. \n He's trying to introduce himself.\n Now let's listen to what he says."));
        textBox->SetText(FText::FromString("\nOops, He's asking for 100 bucks but you ain't got money now \n How you gonna do now? \n"));

        textBox->SetForegroundColor(FLinearColor::White);

        // �ؽ�Ʈ ��Ÿ�� ����
        FTextBlockStyle TextStyle = textBox->WidgetStyle.TextStyle;
        TextStyle.SetColorAndOpacity(FSlateColor(FLinearColor::White));  // �ؽ�Ʈ ������ ������� ����
        TextStyle.SetShadowOffset(FVector2D(1.0f, 1.0f));  // �׸��� ������ ����
        TextStyle.SetShadowColorAndOpacity(FLinearColor(0, 0, 0, 0.5f));  // �׸��� ���� ���� ����

        textBox->SetTextStyle(TextStyle);

        // ����� ������ �����ϰ� ����
        FSlateBrush TransparentBrush;
        TransparentBrush.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0.6f));  // ������ ������ ���

        textBox->WidgetStyle.SetBackgroundImageNormal(TransparentBrush);
        textBox->WidgetStyle.SetBackgroundImageHovered(TransparentBrush);
        textBox->WidgetStyle.SetBackgroundImageFocused(TransparentBrush);
    }
}
