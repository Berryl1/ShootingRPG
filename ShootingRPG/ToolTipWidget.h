#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemTable.h"
#include "ToolTipWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOOTINGRPG_API UToolTipWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetTooltipData(const FItemData& Item);

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemNameText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemDescriptionText;
};