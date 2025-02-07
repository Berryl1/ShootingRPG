#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTINGRPG_API UStatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UStatComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** Set Stats **/
    UPROPERTY(EditAnywhere, Category = "Stat | ST") float MaxHealth = 100.0f;
    UPROPERTY(EditAnywhere, Category = "Stat | ST") float Health = 100.0f;
    UPROPERTY(EditAnywhere, Category = "Stat | ST") float AttackPower = 30.0f;
    UPROPERTY(EditAnywhere, Category = "Stat | ST") float CriticalDmgPercent = 150.0f;

    UPROPERTY(EditAnywhere, Category = "Stat | ED") float MaxStamina = 100.0f;
    UPROPERTY(EditAnywhere, Category = "Stat | ED") float Stamina = 100.0f;
    UPROPERTY(EditAnywhere, Category = "Stat | ED") float HealthRegen = 1.0f;
    UPROPERTY(EditAnywhere, Category = "Stat | ED") float StaminaRegen = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Stat | AG") float DefensePower = 10.0f;
    UPROPERTY(EditAnywhere, Category = "Stat | AG") float MoveSpeed = 300.0f;
    UPROPERTY(EditAnywhere, Category = "Stat | AG") float CriticalChance = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Stat | EXP") int32 Level = 1;
    UPROPERTY(EditAnywhere, Category = "Stat | EXP") float Exp = 0.0f;
    UPROPERTY(EditAnywhere, Category = "Stat | EXP") float MaxExp = 100.0f;

    /** Set Stat Points **/
    UPROPERTY(EditAnywhere, Category = "StatPoints") int32 StatPoints = 0;
    UPROPERTY(EditAnywhere, Category = "StatPoints") int32 Strength = 0;
    UPROPERTY(EditAnywhere, Category = "StatPoints") int32 Agility = 0;
    UPROPERTY(EditAnywhere, Category = "StatPoints") int32 Endurance = 0;

    UFUNCTION(BlueprintCallable, Category = "Stat") void TakeDamage(float Damage);
    UFUNCTION(BlueprintCallable, Category = "Stat") void Heal(float HealAmount);
    UFUNCTION(BlueprintCallable, Category = "Stat") void UseStamina(float StaminaToUse);
    UFUNCTION(BlueprintCallable, Category = "Stat") void RegenHealth(float DeltaTime);
    UFUNCTION(BlueprintCallable, Category = "Stat") void RegenStamina(float DeltaTime);
    UFUNCTION(BlueprintCallable, Category = "Stat") void AddExp(float ExpToAdd);
    UFUNCTION(BlueprintCallable, Category = "Stat") void LevelUp();

    UFUNCTION(BlueprintCallable, Category = "StatPoints") void AddStatPoints(int32 PointsToAdd);
    UFUNCTION(BlueprintCallable, Category = "StatPoints") void SetStrength();
    UFUNCTION(BlueprintCallable, Category = "StatPoints") void SetAgility();
    UFUNCTION(BlueprintCallable, Category = "StatPoints") void SetEndurance();

    UFUNCTION(BlueprintCallable, Category = "StatPoints") void SetAllStats();
    UFUNCTION(BlueprintCallable, Category = "StatPoints") void ResetStatPoints();
};
