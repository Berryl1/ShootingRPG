#pragma once

#include "CoreMinimal.h"
#include "ItemTable.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

class UImage;

UCLASS()
class SHOOTINGRPG_API UItemSlotWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon;

    void InitializeSlot(const FItemData& ItemData);
};

