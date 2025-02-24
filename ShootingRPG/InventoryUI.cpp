#include "InventoryUI.h"
#include "ShootingRPGCharacter.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UInventoryUI::NativeConstruct()
{
    Super::NativeConstruct();

    ItemButtons.SetNum(22);
    ItemCountTexts.SetNum(22);

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

    if (RemoveItem_Bt) {RemoveItem_Bt->
        OnClicked.AddDynamic(this, &UInventoryUI::RemoveClicked); }

    if (Sort_Button) {Sort_Button->
		OnClicked.AddDynamic(this, &UInventoryUI::SortClicked); }

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
            FLinearColor YellowTint = FLinearColor(1.0f, 1.0f, 0.0f, 1.0f); // RGBA (노란색)

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

    ItemCountTexts.SetNum(SortedInventory.Num());
    UE_LOG(LogTemp, Warning, TEXT("SortedInventory.Num() : %d"), SortedInventory.Num());

    for (int32 i = 0; i < SortedInventory.Num(); i++)
    {
        const FItemData& ItemData = SortedInventory[i];

        int32* QuantityPtr = RPGCharacter->ItemQuantities.Find(ItemData.ItemName);
        int32 Quantity = QuantityPtr ? *QuantityPtr : 0;
        
        VisibleItemCount(i);
    }
}



void UInventoryUI::RefreshInventory(const TArray<FItemData>& SortedInventory)
{
    if (SortedInventory.Num() > ItemButtons.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory size exceeds the number of item buttons!"));
		return;
	}

    for (int32 i = 0; i < SortedInventory.Num(); i++)
    {
        if (ItemButtons[i])
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
    }

    UpdateItemCounts(SortedInventory);
}

void UInventoryUI::HideItemCount(int32 Index)
{
    if (ItemCountTexts.IsValidIndex(Index))
    {
        UTextBlock* ItemCountText = ItemCountTexts[Index];

        if (ItemCountText)
        {
            ItemCountText->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UInventoryUI::VisibleItemCount(int32 Index)
{
    if (ItemCountTexts.IsValidIndex(Index))
    {
        UTextBlock* ItemCountText = ItemCountTexts[Index];

        if (ItemCountText)
        {
            ItemCountText->SetVisibility(ESlateVisibility::Visible);
        }
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
    if (Tooltip)
    {
        Tooltip->SetTooltipName(ItemName);
    }
}


void UInventoryUI::OnItemHovered()
{
    if (Tooltip)
    {
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

        if (HoveredButton)
        {
            int32 Index = ItemButtons.IndexOfByKey(HoveredButton);
            if (Index != INDEX_NONE)
            {
                // 플레이어 캐릭터 가져오기
                AShootingRPGCharacter* RPGCharacter = Cast<AShootingRPGCharacter>(GetOwningPlayerPawn());
                if (RPGCharacter && RPGCharacter->Inventory.IsValidIndex(Index))
                {
                    // 인벤토리에서 해당 인덱스의 아이템 이름 가져오기
                    FName ItemName = RPGCharacter ->Inventory[Index].ItemName;
                    UpdateTooltipText(ItemName);
                }
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
}


void UInventoryUI::OnItemUnhovered()
{
    if (Tooltip)
    {
        Tooltip->SetVisibility(ESlateVisibility::Hidden);
    }
}


void UInventoryUI::OnItemClicked()
{
    UButton* ClickedButton = nullptr;

    // 클릭된 버튼 찾기
    for (UButton* Button : ItemButtons)
    {
        if (Button && Button->IsPressed())  // 현재 눌려있는 버튼 찾기
        {
            ClickedButton = Button;
            break;
        }
    }

    if (!ClickedButton)
    {
        UE_LOG(LogTemp, Warning, TEXT("No button is currently pressed."));
        return;
    }

    // 클릭된 버튼의 인덱스 찾기
    int32 Index = ItemButtons.IndexOfByKey(ClickedButton);
    if (Index == INDEX_NONE)
    {
        UE_LOG(LogTemp, Warning, TEXT("Clicked button not found in ItemButtons array!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Item %d clicked!"), Index);

    // 버튼 스타일 변경 (노란색)
    FButtonStyle NewStyle = ClickedButton->WidgetStyle;
    NewStyle.Normal.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));  // 노란색
    ClickedButton->SetStyle(NewStyle);
}

void UInventoryUI::RemoveClicked()
{
    
}

void UInventoryUI::SortClicked()
{
    if (Sort_Button)
    {
        AShootingRPGCharacter* RPGCharacter = Cast<AShootingRPGCharacter>(GetOwningPlayerPawn());
        if (RPGCharacter)
        {
            Sort_Button->SetIsEnabled(false);  // 버튼 비활성화
            RPGCharacter->SortInventory();

            // 0.5초 후 다시 활성화
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
                {
                    if (Sort_Button)
                    {
                        Sort_Button->SetIsEnabled(true);
                    }
                }, 0.5f, false);
        }
    }
}
