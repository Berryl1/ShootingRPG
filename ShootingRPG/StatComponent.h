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
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat | ST") float MaxHealth;
    UPROPERTY(EditAnywhere, Category = "Stat | ST") float DefaultMaxHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, Category = "Stat | ST") float Health;
    UPROPERTY(EditAnywhere, Category = "Stat | ST") float DefaultHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, Category = "Stat | ST") float AttackPower;
    UPROPERTY(EditAnywhere, Category = "Stat | ST") float DefaultAttackPower = 30.0f;

    UPROPERTY(VisibleAnywhere, Category = "Stat | ST") float CriticalDmgPercent;
    UPROPERTY(EditAnywhere, Category = "Stat | ST") float DefaultCriticalDmgPercent = 150.0f;

    UPROPERTY(VisibleAnywhere, Category = "Stat | ED") float MaxStamina;
    UPROPERTY(EditAnywhere, Category = "Stat | ED") float DefaultMaxStamina = 100.0f;

    UPROPERTY(VisibleAnywhere, Category = "Stat | ED") float Stamina;
    UPROPERTY(EditAnywhere, Category = "Stat | ED") float DefaultStamina = 100.0f;

    UPROPERTY(VisibleAnywhere, Category = "Stat | ED") float HealthRegen;
    UPROPERTY(EditAnywhere, Category = "Stat | ED") float DefaultHealthRegen = 1.0f;

    UPROPERTY(VisibleAnywhere, Category = "Stat | ED") float StaminaRegen;
    UPROPERTY(EditAnywhere, Category = "Stat | ED") float DefaultStaminaRegen = 1.0f;

    UPROPERTY(VisibleAnywhere, Category = "Stat | AG") float DefensePower;
    UPROPERTY(EditAnywhere, Category = "Stat | AG") float DefaultDefensePower = 10.0f;

    UPROPERTY(VisibleAnywhere, Category = "Stat | AG") float MoveSpeed;
    UPROPERTY(EditAnywhere, Category = "Stat | AG") float DefaultMoveSpeed = 300.0f;

    UPROPERTY(VisibleAnywhere, Category = "Stat | AG") float CriticalChance;
    UPROPERTY(EditAnywhere, Category = "Stat | AG") float DefaultCriticalChance = 0.0f;


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