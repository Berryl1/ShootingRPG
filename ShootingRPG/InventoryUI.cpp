#include "InventoryUI.h"
#include "ShootingRPGCharacter.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#define INVENTORY_SIZE 22

void UInventoryUI::NativeConstruct()
{
    Super::NativeConstruct();

    ItemButtons.SetNum(INVENTORY_SIZE);
    ItemCountTexts.SetNum(INVENTORY_SIZE);

    for (int32 i = 0; i < 22; i++)
    {
        FString ButtonName = FString::Printf(TEXT("ItemBox_%02d"), i);
        UButton* FoundButton = Cast<UButton>(GetWidgetFromName(FName(*ButtonName)));

        FString TextsName = FString::Printf(TEXT("Item_Count_%02d"), i);
        UTextBlock* FoundText = Cast<UTextBlock>(GetWidgetFromName(FName(*TextsName)));

        if (FoundButton)
        {
    
            ItemButtons[i] = FoundButton;
            ItemButtons[i]->SetIsEnabled(false);

            ItemButtons[i]->OnClicked.AddDynamic(this, &UInventoryUI::OnItemClicked);

            ItemButtons[i]->OnHovered.AddDynamic(this, &UInventoryUI::OnItemHovered);
            ItemButtons[i]->OnUnhovered.AddDynamic(this, &UInventoryUI::OnItemUnhovered);
            
        }

        if (FoundText)
        {
            ItemCountTexts[i] = FoundText; 
            FoundText->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (RemoveItem_Bt)
    {
        RemoveItem_Bt->OnClicked.AddDynamic(this, &UInventoryUI::RemoveClicked);
    }

    if (Equip_Button)
    {
        Equip_Button->OnClicked.AddDynamic(this, &UInventoryUI::EquipClicked);
    }

    if (Sort_Button)
    {
        Sort_Button->OnClicked.AddDynamic(this, &UInventoryUI::SortClicked);
    }

    SetVisibility(ESlateVisibility::Hidden);

    Tooltip->SetVisibility(ESlateVisibility::Hidden);

    Inventory_Weight->SetText(FText::Format(NSLOCTEXT("YourNameSpace", "Weight", "{0} / 100"),
        FText::AsNumber(Current_Weight)));
}


void UInventoryUI::SetItemToGrid(UTexture2D* ItemIcon, int32 Index)
{
    if (ItemIcon && ItemButtons.IsValidIndex(Index))
    {
        UButton* ItemButton = ItemButtons[Index];

        if (ItemButton)
        {
            // Get Button Style
            FButtonStyle& ButtonStyle = ItemButton->WidgetStyle;
            FLinearColor YellowTint = FLinearColor(1.0f, 1.0f, 0.0f, 1.0f); // Yellow Tint

            // Set New Image
            FSlateBrush Brush;
            Brush.SetResourceObject(ItemIcon);
            Brush.ImageSize = FVector2D(80.0f, 80.0f);

            // Set Button Style
            ButtonStyle.Normal = Brush;
            ButtonStyle.Hovered = Brush;
            ButtonStyle.Pressed = Brush;
            ButtonStyle.Pressed.TintColor = FSlateColor(YellowTint);

            ItemButton->SetStyle(ButtonStyle);
            ItemButton->SetIsEnabled(true);
        }
    }
}


void UInventoryUI::UpdateItemCount(int32 Index, int32 Quantity)
{
    if (ItemCountTexts.IsValidIndex(Index))
    {
        UTextBlock* ItemCountText = ItemCountTexts[Index];

        if (!ItemCountText)
        {
            return;
        }

        ItemCountText->SetText(FText::AsNumber(Quantity));
        ItemCountText->SetVisibility(Quantity >= 1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void UInventoryUI::UpdateItemCounts(const TArray<FItemData>& SortedInventory)
{
    AShootingRPGCharacter* RPGCharacter = Cast<AShootingRPGCharacter>(GetOwningPlayerPawn());
    if (!RPGCharacter)
    {
        return;
    }

    for (int32 i = 0; i < SortedInventory.Num() && i < ItemButtons.Num(); i++) // 버튼 개수 내에서만 업데이트
    {
        const FItemData& ItemData = SortedInventory[i];

        int32* QuantityPtr = RPGCharacter->ItemQuantities.Find(ItemData.ItemName);
        int32 Quantity = QuantityPtr ? *QuantityPtr : 0;

        UpdateItemCount(i, Quantity);
    }
}



void UInventoryUI::RefreshInventory(const TArray<FItemData>& SortedInventory)
{
    if (SortedInventory.Num() > ItemButtons.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("Inventory size exceeds the number of item buttons!"));
        return;
    }

    for (int32 i = 0; i < INVENTORY_SIZE; i++)
    {
        if (!ItemButtons[i])
        {
            return;
        }

        if (i < SortedInventory.Num())
        {
            UTexture2D* ItemIcon = SortedInventory[i].ItemIcon;

            FSlateBrush Brush;
            Brush.SetResourceObject(ItemIcon);
            Brush.ImageSize = FVector2D(80.0f, 80.0f);

            ItemButtons[i]->WidgetStyle.Normal = Brush;
            ItemButtons[i]->WidgetStyle.Hovered = Brush;
            ItemButtons[i]->WidgetStyle.Pressed = Brush;
            ItemButtons[i]->SetIsEnabled(true);
        }

        else  // Hide the rest of the buttons
        {
            ItemButtons[i]->SetIsEnabled(false);

            // Set Empty Brush
            FSlateBrush EmptyBrush;
            ItemButtons[i]->WidgetStyle.Normal = EmptyBrush;
            ItemButtons[i]->WidgetStyle.Hovered = EmptyBrush;
            ItemButtons[i]->WidgetStyle.Pressed = EmptyBrush;

            ItemCountTexts[i]->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    UpdateItemCounts(SortedInventory);
}


void UInventoryUI::HideItemCount(int32 Index)
{
    if(!ItemCountTexts.IsValidIndex(Index))
	{
		return;
	}

    UTextBlock* ItemCountText = ItemCountTexts[Index];

    if (ItemCountText)
    {
        ItemCountText->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UInventoryUI::VisibleItemCount(int32 Index)
{
    if (!ItemCountTexts.IsValidIndex(Index))
    {
        return;
    }

    UTextBlock* ItemCountText = ItemCountTexts[Index];

    if (ItemCountText)
    {
        ItemCountText->SetVisibility(ESlateVisibility::Visible);
    }
}


void UInventoryUI::UpdateInventoryDisplay(int32 AddWeight)
{
    Current_Weight = Current_Weight + AddWeight;
    Inventory_Weight->SetText(FText::Format(NSLOCTEXT("YourNameSpace", "Weight", "{0} / 100"),
        FText::AsNumber(Current_Weight)));

}

void UInventoryUI::UpdateTooltipText(FName ItemName)
{
    if (!Tooltip)
    {
        return;
    }

    Tooltip->SetTooltipName(ItemName);
}


void UInventoryUI::OnItemHovered()
{
    if (!Tooltip)
    {
        return;
    }

    // 현재 마우스가 올라간 버튼 찾기
    UButton* HoveredButton = nullptr;
    for (UButton* Button : ItemButtons)
    {
        if (Button && Button->IsHovered())  // 현재 호버 중인 버튼 찾기
        {
            HoveredButton = Button;
            break;
        }
    }

    int32 Index = ItemButtons.IndexOfByKey(HoveredButton);
    if (Index != INDEX_NONE)
    {
        // 플레이어 캐릭터 가져오기
        AShootingRPGCharacter* RPGCharacter = Cast<AShootingRPGCharacter>(GetOwningPlayerPawn());

        if (RPGCharacter && RPGCharacter->Inventory.IsValidIndex(Index))
        {
            // 인벤토리에서 해당 인덱스의 아이템 이름 가져오기
            FName ItemName = RPGCharacter->Inventory[Index].ItemName;
            UpdateTooltipText(ItemName);
            UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *ItemName.ToString())
        }
    }

    float MouseX, MouseY;
    if (GetOwningPlayer()->GetMousePosition(MouseX, MouseY))
    {

        FVector2D TooltipPosition(MouseX - 150.0f, MouseY - 350.0f);
        Tooltip->SetRenderTranslation(TooltipPosition);
    }

    Tooltip->SetVisibility(ESlateVisibility::Visible);
}


void UInventoryUI::OnItemUnhovered()
{
    if (!Tooltip)
    {
        return;
    }

    Tooltip->SetVisibility(ESlateVisibility::Hidden);
    
}


void UInventoryUI::OnItemClicked()
{
    UButton* HoveredButton = nullptr;
    for (UButton* Button : ItemButtons)
    {
        if (Button && Button->IsHovered())  // Find the button that is currently hovered
        {
            HoveredButton = Button;
            break;
        }
    }

    if (!HoveredButton)
	{
		return;
	}

    // Get the index of the item
    int32 Index = ItemButtons.IndexOfByKey(HoveredButton);

    if (Index == INDEX_NONE)
	{
		return;
	}

    UE_LOG(LogTemp, Warning, TEXT("Clicked item at index %d"), Index);

    if (CurrentlySelectedButton && CurrentlySelectedButton != HoveredButton)
	{
		FButtonStyle OldStyle = CurrentlySelectedButton->WidgetStyle;
        OldStyle.Normal.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		CurrentlySelectedButton->SetStyle(OldStyle);
		CurrentlySelectedButton = nullptr;
	}
            
    // Set the style of the button
    FButtonStyle NewStyle = HoveredButton->WidgetStyle;

    if(CurrentlySelectedButton == HoveredButton)
	{
		NewStyle.Normal.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
        CurrentlySelectedButton = nullptr;
	}

	else
	{
		NewStyle.Normal.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
        CurrentlySelectedButton = HoveredButton;
        RemoveItemCountWidget->GetCount = 0;
	}

    AShootingRPGCharacter* RPGCharacter = Cast<AShootingRPGCharacter>(GetOwningPlayerPawn());
    
    if (!RPGCharacter)
    {
        return;
    }
    
    EItemType ItemType = RPGCharacter->Inventory[Index].ItemType;
    FName ItemName = RPGCharacter->Inventory[Index].ItemName;

    if (!RemoveItemCountWidget)
	{
		return;
	}

    if (ItemType == EItemType::Consumable && CurrentlySelectedButton)
	{
        RemoveItemCountWidget->GetCount = 0;
        RemoveItemCountWidget->OnCountTextChanged(FText::FromString("0"));
		RemoveItemCountWidget->SetVisibility(ESlateVisibility::Visible);
		RemoveItemCountWidget->SetMaxCount(RPGCharacter->ItemQuantities[ItemName]);
	}

	HoveredButton->SetStyle(NewStyle);
}

void UInventoryUI::RemoveClicked()
{
    if (!CurrentlySelectedButton)
    {
        UE_LOG(LogTemp, Warning, TEXT("No item selected for removal."));
        return;
    }

    AShootingRPGCharacter* RPGCharacter = Cast<AShootingRPGCharacter>(GetOwningPlayerPawn());
    if (!RPGCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get RPGCharacter."));
        return;
    }

    int32 Index = ItemButtons.IndexOfByKey(CurrentlySelectedButton);
    if (!RPGCharacter->Inventory.IsValidIndex(Index))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid inventory index: %d"), Index);
        return;
    }

    const FItemData& Item = RPGCharacter->Inventory[Index];

    if (!RemoveItemCountWidget)
    {
        UE_LOG(LogTemp, Error, TEXT("RemoveItemCountWidget is null."));
        return;
    }

    int32 InputCount = RemoveItemCountWidget->GetCount;
    if (InputCount <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid item count to remove: %d"), InputCount);
        return;
    }

    RPGCharacter->RemoveItemFromInventory(Item, InputCount);

    // 아이템이 완전히 삭제되었는지 확인 후 버튼 비활성화
    if (!RPGCharacter->ItemQuantities.Contains(Item.ItemName))
    {
        CurrentlySelectedButton = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("Item removed from inventory."));
    }


    // RemoveItemCountWidget 숨기기
    RemoveItemCountWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryUI::EquipClicked()
{

    if (!Equip_Button)
    {
        return;
    }

    if (!CurrentlySelectedButton)
    {
        return;
    }
    AShootingRPGCharacter* RPGCharacter = Cast<AShootingRPGCharacter>(GetOwningPlayerPawn());

    if (!RPGCharacter)
    {
        return;
    }

    // Equip Weapons
    int32 index = ItemButtons.IndexOfByKey(CurrentlySelectedButton);
    
    FItemData EquipItem = RPGCharacter->Inventory[index];
    if (EquipItem.ItemType != EItemType::Weapon)
    {
        return;
    }
    
    RPGCharacter->EquipedWeaponItem = EquipItem;
    RPGCharacter->SetWeaponIcon();
}

void UInventoryUI::SortClicked()
{
    if(!Sort_Button)
	{
		return;
	}

    AShootingRPGCharacter* RPGCharacter = Cast<AShootingRPGCharacter>(GetOwningPlayerPawn());
    
    if (!RPGCharacter)
	{
		return;
	}

    Sort_Button->SetIsEnabled(false);  // Button Disable
    RPGCharacter->SortInventory();

    // Log the inventory list
    UE_LOG(LogTemp, Warning, TEXT("===== Inventory List ====="));
    for (const FItemData& Item : RPGCharacter->Inventory)
    {
        UE_LOG(LogTemp, Warning, TEXT("Item Name: %s, Type: %d, Quantity: %d"),
            *Item.ItemName.ToString(),
            static_cast<int32>(Item.ItemType),
            RPGCharacter->ItemQuantities.Contains(Item.ItemName) ? RPGCharacter->ItemQuantities[Item.ItemName] : 1);
    }
    UE_LOG(LogTemp, Warning, TEXT("========================"));


    // Enable the button after 0.5 seconds
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
        {
            if (Sort_Button)
            {
                Sort_Button->SetIsEnabled(true);
            }
        }, 0.5f, false);

}
