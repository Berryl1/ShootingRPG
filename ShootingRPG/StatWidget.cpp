#include "StatWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "ShootingRPGCharacter.h"
#include "StatComponent.h"

void UStatWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PlayerCharacter = Cast<AShootingRPGCharacter>(GetOwningPlayerPawn());
    if (PlayerCharacter)
    {
        StatComponent = PlayerCharacter->FindComponentByClass<UStatComponent>();
    }

    if (StatComponent)
    {
        UpdateStatDisplay();

        if (ST_Up) ST_Up->OnClicked.AddDynamic(this, &UStatWidget::Increase_ST);
        if (ST_Down) ST_Down->OnClicked.AddDynamic(this, &UStatWidget::Decrease_ST);
        if (AG_Up) AG_Up->OnClicked.AddDynamic(this, &UStatWidget::Increase_AG);
        if (AG_Down) AG_Down->OnClicked.AddDynamic(this, &UStatWidget::Decrease_AG);
        if (ED_Up) ED_Up->OnClicked.AddDynamic(this, &UStatWidget::Increase_ED);
        if (ED_Down) ED_Down->OnClicked.AddDynamic(this, &UStatWidget::Decrease_ED);
        if (Reset) Reset->OnClicked.AddDynamic(this, &UStatWidget::Reset_Stats);
    }
}

void UStatWidget::UpdateStatDisplay()
{
    if (!StatComponent) return;

    Current_ST_Points->SetText(FText::AsNumber(StatComponent->Strength));
    Current_AG_Points->SetText(FText::AsNumber(StatComponent->Agility));
    Current_ED_Points->SetText(FText::AsNumber(StatComponent->Endurance));
    Current_Stat_Point->SetText(FText::Format(NSLOCTEXT("YourNamespace", "StatPoints",
        "스탯 포인트 : {0}"), FText::AsNumber(StatComponent->StatPoints)));

    Current_Max_Health->SetText(FText::Format(NSLOCTEXT("YourNamespace", "MaxHealth", 
        "최대체력 : {0}"), FText::AsNumber(StatComponent->MaxHealth)));

    Current_Max_Stamina->SetText(FText::Format(NSLOCTEXT("YourNamespace", "MaxStamina", 
        "최대기력 : {0}"), FText::AsNumber(StatComponent->MaxStamina)));

    Current_Attack_Power->SetText(FText::Format(NSLOCTEXT("YourNamespace", "AttackPower",
        "공격력 : {0}"), FText::AsNumber(StatComponent->AttackPower)));

    Current_Defense_Power->SetText(FText::Format(NSLOCTEXT("YourNamespace", "DefensePower",
        "방어력 : {0}"), FText::AsNumber(StatComponent->DefensePower)));

    Current_MoveSpeed->SetText(FText::Format( NSLOCTEXT("YourNamespace", "MoveSpeed",
        "이동 속도 : {0}"), FText::AsNumber(StatComponent->MoveSpeed)));

    Current_Critical_Chance->SetText(FText::Format(NSLOCTEXT("YourNamespace", "CriticalChance",
        "치명타 확률 : {0}%"), FText::AsNumber(StatComponent->CriticalChance)));

    Current_Critical_Damage->SetText(FText::Format( NSLOCTEXT("YourNamespace", "CriticalDamage",
        "치명타 데미지 : {0}%"), FText::AsNumber(StatComponent->CriticalDmgPercent)));

    Current_Health_Regen->SetText(FText::Format(NSLOCTEXT("YourNamespace", "HealthRegen",
        "초당 체력 회복 : {0}"), FText::AsNumber(StatComponent->HealthRegen)));

    Current_Stamina_Regen->SetText(FText::Format(NSLOCTEXT("YourNamespace", "StaminaRegen", 
        "초당 기력 회복 : {0}"), FText::AsNumber(StatComponent->StaminaRegen)));
}

void UStatWidget::Increase_ST()
{
    if (StatComponent && StatComponent->StatPoints > 0)
    {
        StatComponent->Strength++;
        StatComponent->StatPoints--;
        StatComponent->SetAllStats();
        UpdateStatDisplay();
    }
}

void UStatWidget::Decrease_ST()
{
    if (StatComponent && StatComponent->Strength > 0)
    {
        StatComponent->Strength--;
        StatComponent->StatPoints++;
        StatComponent->SetAllStats();
        UpdateStatDisplay();
    }
}

void UStatWidget::Increase_AG()
{
    if (StatComponent && StatComponent->StatPoints > 0)
    {
        StatComponent->Agility++;
        StatComponent->StatPoints--;
        StatComponent->SetAllStats();
        UpdateStatDisplay();
    }
}

void UStatWidget::Decrease_AG()
{
    if (StatComponent && StatComponent->Agility > 0)
    {
        StatComponent->Agility--;
        StatComponent->StatPoints++;
        StatComponent->SetAllStats();
        UpdateStatDisplay();
    }
}

void UStatWidget::Increase_ED()
{
    if (StatComponent && StatComponent->StatPoints > 0)
    {
        StatComponent->Endurance++;
        StatComponent->StatPoints--;
        StatComponent->SetAllStats();
        UpdateStatDisplay();
    }
}

void UStatWidget::Decrease_ED()
{
    if (StatComponent && StatComponent->Endurance > 0)
    {
        StatComponent->Endurance--;
        StatComponent->StatPoints++;
        StatComponent->SetAllStats();
        UpdateStatDisplay();
    }
}

void UStatWidget::Reset_Stats()
{
    if (StatComponent)
    {
        StatComponent->ResetStatPoints();
        UpdateStatDisplay();
    }
}
