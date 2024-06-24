// Fill out your copyright notice in the Description page of Project Settings.


#include "UiWidget.h"

#include "ArchVizPlayerController.h"


void UUiWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ComboBoxMode)
	{
		ComboBoxMode->OnSelectionChanged.AddDynamic(this,&UUiWidget::OnComboBoxSelectionChanged);
	}

}

void UUiWidget::OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		PlayerController->ModeChangeHandle(SelectedItem);
	}
}
