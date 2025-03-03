#pragma once

#include "CoreMinimal.h"
#include "ToolTipWidget.h"
#include "RemoveItemCountWidget.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUI.generated.h"

class UTextBlock;
class UButton;
class UToolTipWidget;


UCLASS()
class SHOOTINGRPG_API UInventoryUI : public UUserWidget
{
    GENERATED_BODY()

public:
    UButton* CurrentlySelectedButton = nullptr;
    
    void NativeConstruct();

    UPROPERTY(meta = (BindWidget)) TArray<UButton*> ItemButtons;

    UPROPERTY(meta = (BindWidget)) TArray<UTextBlock*> ItemCountTexts;

    UPROPERTY(meta = (BindWidget)) UButton* RemoveItem_Bt;

    UPROPERTY(meta = (BindWidget)) UButton* Equip_Button;

    UPROPERTY(meta = (BindWidget)) UButton* Sort_Button;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void SetItemToGrid(UTexture2D* ItemIcon, int32 index);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UpdateItemCount(int32 Index, int32 Quantity);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UpdateItemCounts(const TArray<FItemData>& SortedInventory);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void RefreshInventory(const TArray<FItemData>& SortedInventory);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void HideItemCount(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void VisibleItemCount(int32 Index);
    
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UpdateInventoryDisplay(int32 AddWeight);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void OnItemHovered();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void OnItemUnhovered();

    UFUNCTION(BlueprintCallable, Category = "inventory")
    void OnItemClicked();

    UFUNCTION(BlueprintCallable, Category = "inventory")
    void RemoveClicked();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void EquipClicked();

    UFUNCTION(BlueprintCallable, Category = "inventory")
    void SortClicked();

    UFUNCTION(BlueprintCallable, Category = "Tooltip")
    void UpdateTooltipText(FName ItemName);

    UPROPERTY(EditAnywhere, Category = "Inventory") int32 Current_Weight = 0;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UTextBlock* Inventory_Weight;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UToolTipWidget* Tooltip;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) URemoveItemCountWidget* RemoveItemCountWidget;

protected:
    UPROPERTY(meta = (BindWidget))
    class UUniformGridPanel* InventoryGrid;
   
};
