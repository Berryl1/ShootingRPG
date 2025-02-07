#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemTable.h"
#include "Engine/DataTable.h"
#include "ItemActor.generated.h"

UCLASS()
class SHOOTINGRPG_API AItemActor : public AActor
{
    GENERATED_BODY()

public:
    AItemActor();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
    virtual void Tick(float DeltaTime) override;

    // Set item data
    void SetItemData(FName ItemID, UDataTable* ItemDataTable);

    // Get item data
    UFUNCTION(BlueprintCallable, Category = "Item")
    FItemData GetItemData() const;

private:
    // Item data
    UPROPERTY(EditAnywhere, Category = "Item")
    FItemData ItemData;

    // Item data table
    UPROPERTY(EditAnywhere, Category = "Item")
    UDataTable* ItemDataTable;

    // Item ID
    UPROPERTY(EditAnywhere, Category = "Item")
    FName ItemID;

    // Mesh component
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UStaticMeshComponent* MeshComponent;

    // Collision component
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USphereComponent* CollisionComponent;
};
