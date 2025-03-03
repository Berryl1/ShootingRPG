// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidget.h"
#include "Components/Image.h"

void UInGameWidget::SetWeaponIcon(UTexture2D* ItemIcon)
{
	if (!EquipedWeaponIcon)
	{
		return;
	}

	EquipedWeaponIcon->SetBrushFromTexture(ItemIcon);
}
