// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameWidget.h"

#include "UArchVizSaveGame.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

void USaveGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind the button click events
	if (SaveButton)
	{
		SaveButton->OnClicked.AddDynamic(this, &USaveGameWidget::OnSaveButtonClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &USaveGameWidget::OnCancelButtonClicked);
	}
}

void USaveGameWidget::OnSaveButtonClicked()
{
	if (SaveNameInput)
	{
		FString SaveName = SaveNameInput->GetText().ToString();

		
		if (!SaveName.IsEmpty())
		{
			// Create a new save game instance
			UUArchVizSaveGame* SaveGameInstance = Cast<UUArchVizSaveGame>(UGameplayStatics::CreateSaveGameObject(UUArchVizSaveGame::StaticClass()));

			if (SaveGameInstance)
			{
				if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveName, 0))
				{
					UE_LOG(LogTemp, Log, TEXT("Game saved successfully in slot: %s"), *SaveName);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to save game in slot: %s"), *SaveName);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Save name is empty. Please enter a valid save name."));
		}
	}
}

void USaveGameWidget::OnCancelButtonClicked()
{
	// Close or hide the save game widget
	RemoveFromParent();
}