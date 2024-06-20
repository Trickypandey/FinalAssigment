// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WallConstructionWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/ComboBoxString.h"
#include "UiWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALASSIGMENT_API UUiWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
public:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ComboBoxMode;

	

private:
	UPROPERTY()
	UWallConstructionWidget* WallConstructionWidgetInstance;

	UFUNCTION()
	void OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
};
