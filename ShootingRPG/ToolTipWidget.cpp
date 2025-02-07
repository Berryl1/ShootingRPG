#include "ToolTipWidget.h"
#include "Components/TextBlock.h"

void UToolTipWidget::SetTooltipData(const FItemData& ItemData)
{
    if (ItemNameText)
    {
        ItemNameText->SetText(FText::FromName(ItemData.ItemName));
    }
}

void UToolTipWidget::NativeConstruct()
{
    Super::NativeConstruct();
}