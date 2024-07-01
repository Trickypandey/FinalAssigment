// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveGameWidget.h"

#include "ArchVizPlayerController.h"
#include "SlotNamesSaveGame.h"
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

void USaveGameWidget::SaveSlot(FString SlotName)
{
	USlotNamesSaveGame* SlotNamesSaveGame = Cast<USlotNamesSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SlotNamesSaveSlot"), 0));
	if (!SlotNamesSaveGame)
	{
		SlotNamesSaveGame = Cast<USlotNamesSaveGame>(UGameplayStatics::CreateSaveGameObject(USlotNamesSaveGame::StaticClass()));
	}

	if (SlotNamesSaveGame)
	{
		SlotNamesSaveGame->SlotNames.Add(SlotName);

		bool bSaveSuccessful = UGameplayStatics::SaveGameToSlot(SlotNamesSaveGame, TEXT("SlotNamesSaveSlot"), 0);

		if (bSaveSuccessful)
		{
			UE_LOG(LogTemp, Log, TEXT("Slot name successfully saved to slot"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save slot names"));
		}
	}
}

void USaveGameWidget::OnSaveButtonClicked()
{
	FString SaveName = SaveNameInput->GetText().ToString();

	// Check if SaveName is not empty
	if (!SaveName.IsEmpty())
	{
		if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
		{
			if (!CheckSlotAlreadyExist(SaveName))
			{
				if (PlayerController->SaveGame(SaveName))
				{
					UE_LOG(LogTemp, Log, TEXT("Game saved successfully in slot: %s"), *SaveName);

					SaveSlot(SaveName);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to save game in slot: %s"), *SaveName);
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Slot Name Already exists: %s"), *SaveName);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Save name is empty. Please enter a valid save name."));
	}
}

void USaveGameWidget::OnCancelButtonClicked()
{
	// Close or hide the save game widget
	RemoveFromParent();
}

bool USaveGameWidget::CheckSlotAlreadyExist(const FString& SlotName)
{
	USlotNamesSaveGame* SlotNamesSaveGame = Cast<USlotNamesSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SlotNamesSaveSlot"), 0));
	if (!SlotNamesSaveGame)
	{
		SlotNamesSaveGame = Cast<USlotNamesSaveGame>(UGameplayStatics::CreateSaveGameObject(USlotNamesSaveGame::StaticClass()));
	}

	if (SlotNamesSaveGame)
	{
		// Check if the slot name already exists in the array
		return SlotNamesSaveGame->SlotNames.Contains(SlotName);
	}

	return false;
}
