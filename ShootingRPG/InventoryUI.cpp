#include "InventoryUI.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Image.h"

void UInventoryUI::InitializeGrid()
{
    if (InventoryGrid)
    {
        InventoryGrid->ClearChildren(); // 기존의 모든 자식을 삭제합니다.

        // 5x8 그리드 생성
        for (int32 Row = 0; Row < 5; ++Row)
        {
            for (int32 Column = 0; Column < 4; ++Column)
            {
                UImage* NewImage = NewObject<UImage>(this);

                // 이미지 크기 설정 (정사각형)
                FVector2D ImageSize = FVector2D(64.0f, 64.0f); // 원하는 크기로 설정
                NewImage->SetBrushSize(ImageSize);

                // 기본적으로 투명하게 설정
                NewImage->SetRenderOpacity(0.0f);

                UUniformGridSlot* GridSlot = InventoryGrid->AddChildToUniformGrid(NewImage, Row, Column);
                GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
                GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
            }
        }
    }
}

void UInventoryUI::AddItemToGrid(UTexture2D* ItemIcon, int32 Row, int32 Column)
{
    if (InventoryGrid)
    {
        // 그리드의 특정 위치에 이미지를 설정합니다.
        int32 Index = Row * Column + Column - 1; // 4열 그리드 인덱스 계산

        if (UImage* ItemImage = Cast<UImage>(InventoryGrid->GetChildAt(Index)))
        {
            if (ItemIcon)
            {
                ItemImage->SetBrushFromTexture(ItemIcon);
                ItemImage->SetRenderOpacity(1.0f);

                UE_LOG(LogTemp, Log, TEXT("행: %d, 열: %d"), Row, Column);
            }

        }
    }
}
