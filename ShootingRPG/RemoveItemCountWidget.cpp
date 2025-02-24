#include "RemoveItemCountWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"

URemoveItemCountWidget::URemoveItemCountWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void URemoveItemCountWidget::SetMaxCount(int32 MaxCount)
{
	GetMaxCount = MaxCount;
}

void URemoveItemCountWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Count_Text)
	{
		Count_Text->OnTextChanged.AddDynamic(this, &URemoveItemCountWidget::OnCountTextChanged);
	}

	if (Check_Button)
	{
		Check_Button->OnClicked.AddDynamic(this, &URemoveItemCountWidget::OnCheckButtonClicked);
	}
}

void URemoveItemCountWidget::OnCountTextChanged(const FText& Text)
{
	FString InputText = Text.ToString();
	int32 InputCount = FCString::Atoi(*InputText);

	// Set the text to 0 if the input is less than 0
	if (InputCount < 0)
	{
		Count_Text->SetText(FText::FromString("0"));
	}
	// if the input is greater than MaxCount then set the text to MaxCount
	else if (InputCount > GetMaxCount)
	{
		Count_Text->SetText(FText::FromString(FString::FromInt(GetMaxCount)));
	}
	else
	{
		Count_Text->SetText(Text);
	}
}

void URemoveItemCountWidget::OnCheckButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	GetCount = FCString::Atoi(*Count_Text->GetText().ToString());
}
