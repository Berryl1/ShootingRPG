#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUI.generated.h"

UCLASS()
class SHOOTINGRPG_API UInventoryUI : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void InitializeGrid();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void AddItemToGrid(UTexture2D* ItemIcon, int32 Row, int32 Column);

protected:
    UPROPERTY(meta = (BindWidget))
    class UUniformGridPanel* InventoryGrid;
};
