// Fill out your cpyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatComponent.h"
#include "ShootingRPGCharacter.h"
#include "Blueprint/UserWidget.h"
#include "StatWidget.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class SHOOTINGRPG_API UStatWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;

	UStatComponent* StatComponent;
public:

	/** Binding Text **/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UTextBlock* Current_ST_Points;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UTextBlock* Current_AG_Points;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UTextBlock* Current_ED_Points;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UTextBlock* Current_Stat_Point;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UTextBlock* Current_Max_Health;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UTextBlock* Current_Max_Stamina;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UTextBlock* Current_Attack_Power;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UTextBlock* Current_Defense_Power;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UTextBlock* Current_MoveSpeed;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UTextBlock* Current_Critical_Chance;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UTextBlock* Current_Critical_Damage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UTextBlock* Current_Health_Regen;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UTextBlock* Current_Stamina_Regen;

	/** Binding Buttons **/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UButton* ST_Up;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UButton* ST_Down;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UButton* AG_Up;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UButton* AG_Down;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UButton* ED_Up;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UButton* ED_Down;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UButton* Reset;

	/** Button Function **/
	UFUNCTION() void Increase_ST();
	UFUNCTION() void Decrease_ST();
	UFUNCTION() void Increase_AG();
	UFUNCTION() void Decrease_AG();
	UFUNCTION() void Increase_ED();
	UFUNCTION() void Decrease_ED();
	UFUNCTION() void Reset_Stats();

	void UpdateStatDisplay();

private:
	AShootingRPGCharacter* PlayerCharacter;
};
