// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ComboBox.h"
#include "Components/ComboBoxString.h"
#include "LoadSlotWidget.generated.h"


/**
 * 
 */
UCLASS()
class FINALASSIGMENT_API ULoadSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* SlotListComboBox;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadSlotButton;

private:
	UFUNCTION()
	void OnLoadSlotButtonClicked();

	void PopulateSaveSlots();
	
};
