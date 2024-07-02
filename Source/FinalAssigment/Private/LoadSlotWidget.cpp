// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadSlotWidget.h"

#include "ArchVizPlayerController.h"
#include "SlotNamesSaveGame.h"
#include "Kismet/GameplayStatics.h"

void ULoadSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotListComboBox)
	{
		PopulateSaveSlots();
	}

	if (LoadSlotButton)
	{
		LoadSlotButton->OnClicked.AddDynamic(this, &ULoadSlotWidget::OnLoadSlotButtonClicked);
	}

}

void ULoadSlotWidget::PopulateSaveSlots()
{
	USlotNamesSaveGame* SlotNamesSaveGame = Cast<USlotNamesSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SlotNamesSaveSlot"), 0));
	if (SlotNamesSaveGame)
	{
		if (SlotNamesSaveGame->SlotNames.IsEmpty())
		{
			if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
			{
				PlayerController->BroadcastToast("No Templates To Load");
			}

			return;
		}
		SlotListComboBox->ClearOptions();
		for (const FString& SlotName : SlotNamesSaveGame->SlotNames)
		{
			SlotListComboBox->AddOption(SlotName);
		}
	}
}

void ULoadSlotWidget::OnLoadSlotButtonClicked()
{
	FString SelectedSlot = SlotListComboBox->GetSelectedOption();

	if (!SelectedSlot.IsEmpty())
	{
		if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
		{
			PlayerController->LoadGame(SelectedSlot);
			RemoveFromParent();  // Close the widget after loading
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No slot selected. Please select a slot to load."));
	}
}

FReply ULoadSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	FVector2D LocalMousePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	// Check if click is within the combo box
	if (IsPointWithinWidget(SlotListComboBox, LocalMousePosition) ||
		IsPointWithinWidget(LoadSlotButton, LocalMousePosition))
	{
		// Do nothing if the click is inside the combo box or button
		return FReply::Handled();
	}

	// Remove the widget if click is outside
	RemoveFromParent();
	return FReply::Handled();
}

bool ULoadSlotWidget::IsPointWithinWidget(UWidget* Widget, const FVector2D& Point)
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