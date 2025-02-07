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

	// Inventory UI 생성 (이미 잘 구현됨)
	if (InventoryUIClass)
	{
		InventoryUIInstance = CreateWidget<UInventoryUI>(GetWorld(), InventoryUIClass);
		if (InventoryUIInstance)
		{
			InventoryUIInstance->AddToViewport();
			InventoryUIInstance->InitializeGrid();
			InventoryUIInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	// Stat Widget 생성 – 생성자를 대신하여 BeginPlay()에서 생성
	if (StatWidgetClass)
	{
		StatWidgetInstance = CreateWidget<UStatWidget>(GetWorld(), StatWidgetClass);
		if (StatWidgetInstance)
		{
			StatWidgetInstance->AddToViewport();
			StatWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AShootingRPGCharacter::AddItemToInventory(FItemData NewItem)
{
	if (InventoryUIInstance)
	{
		Inventory.Add(NewItem);
		OnInventoryUpdated.Broadcast();

		int32 Row = Inventory.Num() / 4; // 행 계산
		int32 Column = Inventory.Num() % 5; // 열 계산
		InventoryUIInstance->AddItemToGrid(NewItem.ItemIcon, Row, Column); // ItemIcon을 그리드에 추가
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

void AShootingRPGCharacter::DebugFunction()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController || !StatWidgetInstance) return;

	if (StatWidgetInstance->IsVisible())
	{
		// 위젯 숨김 + 마우스 커서 숨김
		StatWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());  // 게임 모드로 변경
	}
	else
	{
		if (InventoryUIInstance && InventoryUIInstance->IsVisible())
		{
			InventoryUIInstance->SetVisibility(ESlateVisibility::Hidden);
		}

		// 위젯 표시 + 마우스 커서 보이게 설정
		StatWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly());  // UI 모드로 변경
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
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Null"));
		UE_LOG(LogTemp, Warning, TEXT("Z 버튼 눌림 - PickUpItem 실행"));
	}
}

void AShootingRPGCharacter::ToggleInventory()
{

	if (InventoryUIInstance->IsVisible())
	{
		InventoryUIInstance->SetVisibility(ESlateVisibility::Hidden);
	}
	
	else
	{	
		if (StatWidgetInstance->IsVisible())
		{
			StatWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
		InventoryUIInstance->SetVisibility(ESlateVisibility::Visible);
	}
}


void AShootingRPGCharacter::SetOverlappingItem(AItemActor* NewItem)
{
	OverlappingItem = NewItem;
}
