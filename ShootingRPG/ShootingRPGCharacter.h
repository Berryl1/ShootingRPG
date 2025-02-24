// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "ItemTable.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ShootingRPGCharacter.generated.h"

class UStatWidget;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UInventoryUI;
class UStatComponent;
struct FInputActionValue;



DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AShootingRPGCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PickupItemAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleInventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DebugAction;

public:
	AShootingRPGCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

private:
	AItemActor* OverlappingItem;


protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DebugFunction();
	
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FItemData> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<FName, int32> ItemQuantities;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemToInventory(FItemData NewItem);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemFromInventory(FName ItemName, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<FItemData>& GetInventory() const { return Inventory; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UStatWidget> StatWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UInventoryUI> InventoryUIClass;

	UPROPERTY()
	UInventoryUI* InventoryUIInstance;

	UPROPERTY()
	UStatWidget* StatWidgetInstance;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleInventory();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	UStatComponent* StatComponent;

	void SetOverlappingItem(AItemActor* NewItem);

	void PickUpItem();

	void SortInventory();



	virtual void Tick(float DeltaTime) override;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	
};

