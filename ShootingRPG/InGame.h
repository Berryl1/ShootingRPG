// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGame.generated.h"


class UImage;
/**
 * 
 */
UCLASS()
class SHOOTINGRPG_API UInGame : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UImage* WeaponIcon;
};

