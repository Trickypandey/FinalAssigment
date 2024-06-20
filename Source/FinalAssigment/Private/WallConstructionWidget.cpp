// Fill out your copyright notice in the Description page of Project Settings.


#include "WallConstructionWidget.h"

#include "ArchVizPlayerController.h"

void UWallConstructionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind toggle door button click event
	if (ToggleDoorButton)
	{
		ToggleDoorButton->OnClicked.AddDynamic(this, &UWallConstructionWidget::OnToggleDoorClicked);
	}
}

void UWallConstructionWidget::OnToggleDoorClicked()
{
	bIsAddingDoor = !bIsAddingDoor;
	if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
	{
		PlayerController->SetIsAddingDoor(bIsAddingDoor);
	}
}