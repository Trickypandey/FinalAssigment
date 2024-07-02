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
			if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
			{
				PlayerController->BroadcastToast("Successfully Saved");
				RemoveFromParent();
			}
			
		}
		else
		{
			if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
			{
				PlayerController->BroadcastToast("Save Was Not Successful");
			}
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
					
					SaveSlot(SaveName);
					
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to save game in slot: %s"), *SaveName);
				}
			}
			else
			{
				
					PlayerController->BroadcastToast("Slot Name Already exists");
				
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


FReply USaveGameWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	FVector2D LocalMousePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	// Check if click is within the combo box
	if (IsPointWithinWidget(SaveNameInput, LocalMousePosition) ||
		IsPointWithinWidget(CancelButton, LocalMousePosition) ||
		IsPointWithinWidget(SaveButton, LocalMousePosition))
	{
		// Do nothing if the click is inside the combo box or button
		return FReply::Handled();
	}

	// Remove the widget if click is outside
	RemoveFromParent();
	return FReply::Handled();
}

bool USaveGameWidget::IsPointWithinWidget(UWidget* Widget, const FVector2D& Point)
{
	if (!Widget) return false;

	// Get the widget's geometry
	FGeometry WidgetGeometry = Widget->GetCachedGeometry();
	FVector2D LocalSize = WidgetGeometry.GetLocalSize();
	FVector2D TopLeft = WidgetGeometry.GetAccumulatedRenderTransform().TransformPoint(FVector2D::ZeroVector);
	FVector2D BottomRight = WidgetGeometry.GetAccumulatedRenderTransform().TransformPoint(LocalSize);

	// Check if the point is within the widget's bounds
	return Point.X >= TopLeft.X && Point.X <= BottomRight.X &&
		Point.Y >= TopLeft.Y && Point.Y <= BottomRight.Y;
}