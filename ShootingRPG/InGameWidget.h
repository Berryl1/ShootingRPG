// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

/**
 * 
 */
class UImage;

UCLASS()
class SHOOTINGRPG_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UImage* EquipedWeaponIcon;

	UFUNCTION(BlueprintCallable)
	void SetWeaponIcon(UTexture2D* ItemIcon);
};
