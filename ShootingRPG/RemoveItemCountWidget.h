#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RemoveItemCountWidget.generated.h"

class UButton;
class UEditableTextBox;

UCLASS()
class SHOOTINGRPG_API URemoveItemCountWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 생성자
	URemoveItemCountWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void OnCountTextChanged(const FText& Text);

	UFUNCTION(BlueprintCallable, Category = "Count")
	void SetMaxCount(int32 MaxCount);

	int32 GetMaxCount = 0;

	int32 GetCount = 0;

protected:
	virtual void NativeConstruct() override;

private:
	// 입력을 숫자로 제한하는 함수
	UFUNCTION()
	void OnCheckButtonClicked();

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* Count_Text;

	UPROPERTY(meta = (BindWidget))
	UButton* Check_Button;
};
