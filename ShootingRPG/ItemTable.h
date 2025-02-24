#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "ItemTable.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Weapon UMETA(DisplayName = "Weapon"),
    Armor UMETA(DisplayName = "Armor"),
    Consumable UMETA(DisplayName = "Consumable"),
    QuestItem UMETA(DisplayName = "QuestItem")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    LongWeapon UMETA(DisplayName = "Long Weapon"),
    CloseWeapon UMETA(DisplayName = "Close Weapon"),
    SpecialWeapon UMETA(DisplayName = "Special Weapon"),
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

    // items data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float Weight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Quantity = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* ItemIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UStaticMesh* ItemMesh;

    // item stats
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Damage = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Defense = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float AdditionalEffect = 0.0f;

    // weapon type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "ItemType == EItemType::Weapon"))
    EWeaponType WeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "ItemType == EItemType::Weapon && WeaponType == EWeaponType::LongWeapon"))
    int32 Ammo = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "ItemType == EItemType::Weapon && WeaponType == EWeaponType::CloseWeapon"))
    float AttackSpeed = 1.0f;

    // consumable stats
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable", meta = (EditCondition = "ItemType == EItemType::Consumable"))
    float HealthRestore = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable", meta = (EditCondition = "ItemType == EItemType::Consumable"))
    float StaminaRestore = 0.0f;

    // quest item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestItem", meta = (EditCondition = "ItemType == EItemType::QuestItem"))
    FString QuestID;
};

UCLASS()
class SHOOTINGRPG_API UItemTable : public UDataTable
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Table")
    UDataTable* ItemDataTable;
};
