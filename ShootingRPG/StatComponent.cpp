#include "StatComponent.h"
#include "Math/UnrealMathUtility.h"

UStatComponent::UStatComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    MaxHealth = DefaultMaxHealth;
    Health = DefaultHealth;
    AttackPower = DefaultAttackPower;
    CriticalDmgPercent = DefaultCriticalDmgPercent;

    MaxStamina = DefaultMaxStamina;
    Stamina = DefaultStamina;
    HealthRegen = DefaultHealthRegen;
    StaminaRegen = DefaultStaminaRegen;

    DefensePower = DefaultDefensePower;
    MoveSpeed = DefaultMoveSpeed;
    CriticalChance = DefaultCriticalChance;
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
    MaxHealth = DefaultMaxHealth + Strength * 10.0f;
    AttackPower = DefaultAttackPower + Strength * 2.0f;
    CriticalDmgPercent = DefaultCriticalDmgPercent + Strength * 1.0f;
}

void UStatComponent::SetEndurance()
{
    MaxStamina = DefaultMaxStamina + Endurance * 5.0f;
    HealthRegen = DefaultHealthRegen + Endurance * 0.1f;
    StaminaRegen = DefaultStaminaRegen + Endurance * 0.1f;
}

void UStatComponent::SetAgility()
{
    DefensePower = DefaultDefensePower + Agility * 1.0f;
    MoveSpeed = DefaultMoveSpeed + Agility * 3.0f;
    CriticalChance = DefaultCriticalChance + Agility * 1.5f;
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