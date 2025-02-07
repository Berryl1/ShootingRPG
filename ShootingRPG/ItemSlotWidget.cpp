#include "ItemSlotWidget.h"
#include "Components/Image.h"

void UItemSlotWidget::InitializeSlot(const FItemData& ItemData)
{
    UE_LOG(LogTemp, Log, TEXT("InitializeSlot 호출됨: ItemName = %s"), *ItemData.ItemName.ToString());

    if (ItemIcon && ItemData.ItemIcon)
    {
        ItemIcon->SetBrushFromTexture(ItemData.ItemIcon);
        UE_LOG(LogTemp, Log, TEXT("아이템 아이콘 설정됨: %s"), *ItemData.ItemIcon->GetName());
    }
    else
    {
        if (!ItemIcon)
        {
            UE_LOG(LogTemp, Warning, TEXT("아이템 아이콘 설정 실패: ItemIcon 위젯이 null입니다."));
        }
        if (!ItemData.ItemIcon)
        {
            UE_LOG(LogTemp, Warning, TEXT("아이템 아이콘 설정 실패: ItemIcon 데이터가 null입니다."));
        }
    }
}
