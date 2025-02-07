#include "ItemActor.h"
#include "Engine/DataTable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "ShootingRPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AItemActor::AItemActor()
{
    FString ItemDatapath = TEXT("/Script/Engine.DataTable'/Game/_Item/Data/ItemDataTable.ItemDataTable'");
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(*ItemDatapath);
    if (DataTable.Succeeded())
    {
        ItemDataTable = DataTable.Object;
    }

    PrimaryActorTick.bCanEverTick = true;

    // mesh component
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // collision component
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetupAttachment(RootComponent);
    CollisionComponent->SetSphereRadius(100.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
}

void AItemActor::BeginPlay()
{
    Super::BeginPlay();

    // set item data
    SetItemData(ItemID, ItemDataTable);

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

void AItemActor::SetItemData(FName NewItemID, UDataTable* NewItemDataTable)
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
}

FItemData AItemActor::GetItemData() const
{
    return ItemData;
}
