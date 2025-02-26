#include "ItemActor.h"
#include "Engine/DataTable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "ShootingRPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AItemActor::AItemActor()
{

    PrimaryActorTick.bCanEverTick = true;

    // mesh component
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // collision component
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetupAttachment(RootComponent);
    CollisionComponent->SetSphereRadius(100.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));

    static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/_Item/Data/ItemDataTable.ItemDataTable"));
    if (DataTable.Succeeded())
    {
        ItemDataTable = DataTable.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ItemDataTable not found!"));
        ItemDataTable = nullptr;
    }
}

void AItemActor::BeginPlay()
{
    Super::BeginPlay();

    // set item data
    SetItemData(ItemID, ItemDataTable, ItemData.Quantity);

    // collision event
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlapBegin);
    CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AItemActor::OnOverlapEnd);

    UE_LOG(LogTemp, Warning, TEXT("BeginPlay - ItemID: %s"), *ItemID.ToString());
    UE_LOG(LogTemp, Warning, TEXT("BeginPlay - DataTable: %s"), ItemDataTable ? TEXT("Valid") : TEXT("Invalid"));
}

void AItemActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AShootingRPGCharacter* Character = Cast<AShootingRPGCharacter>(OtherActor);
    if (Character)
    {
        Character->SetOverlappingItem(this);
    }
}

void AItemActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    AShootingRPGCharacter* Character = Cast<AShootingRPGCharacter>(OtherActor);
    if (Character)
    {
        Character->SetOverlappingItem(nullptr);
    }
}

void AItemActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AItemActor::SetItemData(FName NewItemID, UDataTable* NewItemDataTable, int32 Quantity)
{
    if (NewItemDataTable)
    {
        FItemData* Data = NewItemDataTable->FindRow<FItemData>(NewItemID, TEXT(""));
        if (Data)
        {
            ItemData = *Data;
            ItemID = NewItemID;
            ItemDataTable = NewItemDataTable;
        }
    }

    MeshComponent->SetStaticMesh(ItemData.ItemMesh);
    ItemData.Quantity = Quantity;
    UE_LOG(LogTemp, Warning, TEXT("Item Quantity : %d"), Quantity);
}

FItemData AItemActor::GetItemData() const
{
    return ItemData;
}
