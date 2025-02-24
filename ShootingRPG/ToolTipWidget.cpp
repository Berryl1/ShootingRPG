#include "ToolTipWidget.h"
#include "Components/TextBlock.h"


void UToolTipWidget::SetTooltipName(FName ItemName)
{
    if (ItemNameText)
    {
        ItemNameText->SetText(FText::FromName(ItemName));
    }
}

