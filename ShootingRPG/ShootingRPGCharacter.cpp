// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootingRPGCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ItemActor.h"
#include "StatComponent.h"
#include "InventoryUI.h"
#include "StatWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AShootingRPGCharacter

AShootingRPGCharacter::AShootingRPGCharacter()
{

	// Create Stat Component
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	PrimaryActorTick.bCanEverTick = true;
	OverlappingItem = nullptr;
}

void AShootingRPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (!InventoryUIClass)
	{
		return;
	}

	InventoryUIInstance = CreateWidget<UInventoryUI>(GetWorld(), InventoryUIClass);
	if (InventoryUIInstance)
	{
		InventoryUIInstance->AddToViewport();
		InventoryUIInstance->SetVisibility(ESlateVisibility::Hidden);
	}

	if (!StatWidgetClass)
	{
		return;
	}

	StatWidgetInstance = CreateWidget<UStatWidget>(GetWorld(), StatWidgetClass);
	if (StatWidgetInstance)
	{
		StatWidgetInstance->AddToViewport();
		StatWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}




//////////////////////////////////////////////////////////////////////////
// Input

void AShootingRPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShootingRPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShootingRPGCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShootingRPGCharacter::Look);
	
		// Pick up item
		EnhancedInputComponent->BindAction(PickupItemAction, ETriggerEvent::Started, this, &AShootingRPGCharacter::PickUpItem);
	
		// Toggle Inventory
		EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AShootingRPGCharacter::ToggleInventory);
	
		EnhancedInputComponent->BindAction(DebugAction, ETriggerEvent::Started, this, &AShootingRPGCharacter::DebugFunction);
	}
	
}

void AShootingRPGCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AShootingRPGCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AShootingRPGCharacter::PickUpItem()
{
	if (OverlappingItem)
	{
		FString ItemInfo = FString::Printf(TEXT("OverlappingItem: %s"), *OverlappingItem->GetItemData().ItemName.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, ItemInfo);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemInfo);
		// item data
		FItemData ItemData = OverlappingItem->GetItemData();

		// inventory add
		AddItemToInventory(ItemData);
		UE_LOG(LogTemp, Warning, TEXT("Item Added: %s"), *OverlappingItem->GetItemData().ItemName.ToString());

		// destroy item
		OverlappingItem->Destroy();
		OverlappingItem = nullptr;
	}
}

// sort Inventory
void AShootingRPGCharacter::SortInventory()
{
	// Sort Inventory by Item Type
	Inventory.Sort([](const FItemData& A, const FItemData& B)
	{
		if (A.ItemType == B.ItemType)
		{
				return A.ItemName.Compare(B.ItemName) < 0;
		}
		
		return A.ItemType < B.ItemType;

		});

	if(InventoryUIInstance)
	{
		InventoryUIInstance->RefreshInventory(Inventory);
	}

	UE_LOG(LogTemp, Warning, TEXT("Inventory Sorted :"));
	for(const FItemData& Item : Inventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Name: %s, ItemType: %d"), *Item.ItemName.ToString(), (int32)Item.ItemType);
	}

}

void AShootingRPGCharacter::AddItemToInventory(FItemData NewItem)
{
	if (!InventoryUIInstance)
	{
		return;
	}

	FName ItemName = NewItem.ItemName;
	int32 Index = -1;

	if (NewItem.ItemType == EItemType::Consumable || NewItem.ItemType == EItemType::QuestItem)
	{
		for (int32 i = 0; i < Inventory.Num(); i++)
		{
			if (Inventory[i].ItemName == ItemName)
			{
				Index = i;
				break;
			}
		}

		if (Index == -1)
		{
			Index = Inventory.Num();
			Inventory.Add(NewItem);  // Add New Item
			ItemQuantities.Add(ItemName, NewItem.Quantity);  // Set Item Count
			InventoryUIInstance->SetItemToGrid(NewItem.ItemIcon, Index); // Update UI
		}
		else
		{
			ItemQuantities[ItemName] += NewItem.Quantity; // Increase Item Count
			InventoryUIInstance->UpdateItemCount(Index, ItemQuantities[ItemName]); // set count
		}
	}
	// if New Item, Add Invntory
	else
	{
		InventoryUIInstance->SetItemToGrid(NewItem.ItemIcon, Inventory.Num());
		Inventory.Add(NewItem);
	}

	// Update Inventory UI Weight
	InventoryUIInstance->UpdateInventoryDisplay(NewItem.Weight * NewItem.Quantity);
	InventoryUIInstance->RefreshInventory(Inventory);
}

void AShootingRPGCharacter::RemoveItemFromInventory(FItemData RemoveItem, int32 RemoveQuantity)
{
	// Check InventoryUIInstance
	if (!InventoryUIInstance)
	{
		return;
	}

	FName ItemName = RemoveItem.ItemName;
	if (!ItemQuantities.Contains(ItemName))
	{
		UE_LOG(LogTemp, Error, TEXT("Item not found in inventory: %s"), *ItemName.ToString());
		return;
	}

	// Decrease Item Quantity
	ItemQuantities[ItemName] -= RemoveQuantity;

	// update UI Weight
	InventoryUIInstance->UpdateInventoryDisplay(-RemoveItem.Weight * RemoveQuantity);
	
	// Remove Item
	if (ItemQuantities[ItemName] <= 0)
	{
		ItemQuantities.Remove(ItemName);

		int32 Index = Inventory.IndexOfByPredicate([&](const FItemData& Item) { return Item.ItemName == ItemName; });

		if (Index != INDEX_NONE && Inventory.IsValidIndex(Index))
		{
			Inventory.RemoveAt(Index);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Item Removed: %s, Remaining Quantity: %d"), *ItemName.ToString(),
		ItemQuantities.Contains(ItemName) ? ItemQuantities[ItemName] : 0);
	if (Inventory.Num() >= 0)
	{
		InventoryUIInstance->RefreshInventory(Inventory);
	}

	// Item Actor Spawn
	if (ItemActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemActorClass is valid"));
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
		FRotator SpawnRotation = GetActorRotation();

		AItemActor* ItemActor = GetWorld()->SpawnActor<AItemActor>(ItemActorClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (ItemActor)
		{
			// find RemoveItem Row ID

			ItemActor->SetItemData(RemoveItem.ItemName, ItemDataTable, RemoveQuantity);
			UE_LOG(LogTemp, Warning, TEXT("ItemActor Spawned: %s"), *RemoveItem.ItemName.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemActorClass is invalid"));
	}

}

void AShootingRPGCharacter::ToggleInventory()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController || !StatWidgetInstance) return;

	if (InventoryUIInstance->IsVisible())
	{
		InventoryUIInstance->SetVisibility(ESlateVisibility::Hidden);
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
	
	else
	{	
		if (StatWidgetInstance->IsVisible())
		{
			StatWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
		InventoryUIInstance->SetVisibility(ESlateVisibility::Visible);
		PlayerController->bShowMouseCursor = true;
	}
}

void AShootingRPGCharacter::DebugFunction()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController || !StatWidgetInstance) return;

	if (StatWidgetInstance->IsVisible())
	{
		// Hide Widget + Hide Mouse Cursor
		StatWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());  // Change Gamemode
	}
	else
	{
		if (InventoryUIInstance && InventoryUIInstance->IsVisible())
		{
			InventoryUIInstance->SetVisibility(ESlateVisibility::Hidden);
		}

		// Visible Widget + Show Mouse Cursor
		StatWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		PlayerController->bShowMouseCursor = true;
	}
}

void AShootingRPGCharacter::SetOverlappingItem(AItemActor* NewItem)
{
	OverlappingItem = NewItem;
}
