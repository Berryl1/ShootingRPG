#include "StatComponent.h"
#include "Math/UnrealMathUtility.h"

UStatComponent::UStatComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UStatComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    RegenHealth(DeltaTime);
    RegenStamina(DeltaTime);
}

void UStatComponent::TakeDamage(float Damage)
{
    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
}

void UStatComponent::Heal(float HealAmount)
{
    Health = FMath::Clamp(Health + HealAmount, 0.0f, MaxHealth);
}

void UStatComponent::UseStamina(float StaminaToUse)
{
    Stamina = FMath::Clamp(Stamina - StaminaToUse, 0.0f, MaxStamina);
}

void UStatComponent::RegenHealth(float DeltaTime)
{
    Health = FMath::Clamp(Health + HealthRegen * DeltaTime, 0.0f, MaxHealth);
}

void UStatComponent::RegenStamina(float DeltaTime)
{
    Stamina = FMath::Clamp(Stamina + StaminaRegen * DeltaTime, 0.0f, MaxStamina);
}

void UStatComponent::AddExp(float ExpToAdd)
{
    Exp += ExpToAdd;
    if (Exp >= MaxExp)
    {
        Exp -= MaxExp;
        LevelUp();
        MaxExp = 100.0f * FMath::Pow(1.1f, Level);
    }
}

void UStatComponent::LevelUp()
{
    Level++;
    StatPoints += 5;
}

void UStatComponent::AddStatPoints(int32 PointsToAdd)
{
    StatPoints += PointsToAdd;
}

void UStatComponent::SetStrength()
{
    MaxHealth = 100.0f + Strength * 10.0f;
    AttackPower = 30.0f + Strength * 2.0f;
    CriticalDmgPercent = 150.0f + Strength * 1.0f;
}

void UStatComponent::SetEndurance()
{
    MaxStamina = 100.0f + Endurance * 5.0f;
    HealthRegen = 1.0f + Endurance * 0.1f;
    StaminaRegen = 1.0f + Endurance * 0.1f;
}

void UStatComponent::SetAgility()
{
    DefensePower = 10.0f + Agility * 1.0f;
    MoveSpeed = 300.0f + Agility * 3.0f;
    CriticalChance = 4.0f + Agility * 1.5f;
}

void UStatComponent::SetAllStats()
{
    SetStrength();
    SetEndurance();
    SetAgility();
}

void UStatComponent::ResetStatPoints()
{
    StatPoints = Strength + Agility + Endurance + StatPoints;
    Strength = 0;
    Agility = 0;
    Endurance = 0;
    SetAllStats();
}
