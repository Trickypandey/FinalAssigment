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

	if (SaveButton)
	{
        SaveButton->OnClicked.AddDynamic(this, &UUiWidget::OnSaveButtonClickHandle);
	}

	if (LoadButton)
	{
		
        LoadButton->OnClicked.AddDynamic(this, &UUiWidget::OnLoadButtonClickHandle);
	}

}

void UUiWidget::OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer());
	if (PlayerController)
	{

        // Map the selected item to the enum value
        EModes SelectedMode ;

        if (SelectedItem == "Road Creation Mode")
        {
            SelectedMode = EModes::RoadConstruction;
        }
        else if (SelectedItem == "Wall Creation Mode")
        {
            SelectedMode = EModes::WallCreation;
        }
        else if (SelectedItem == "Interior Design Mode")
        {
            SelectedMode = EModes::InteriorDesign;
        }

        // Call the method to change the mode
        
		PlayerController->ModeChangeHandle(SelectedMode);
	}
}

void UUiWidget::OnLoadButtonClickHandle()
{
    if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
    {
        PlayerController->LoadGame();
    }
}

void UUiWidget::OnSaveButtonClickHandle()
{
    if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
    {
        PlayerController->SaveGame();
    }

}
