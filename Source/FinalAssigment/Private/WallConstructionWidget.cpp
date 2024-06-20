// Fill out your copyright notice in the Description page of Project Settings.


#include "WallConstructionWidget.h"

#include "ArchVizPlayerController.h"
#include "SlabActor.h"
#include "FinalAssigment/WallActor.h"

void UWallConstructionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind toggle door button click event
	if (ToggleDoorButton)
	{
		ToggleDoorButton->OnClicked.AddDynamic(this, &UWallConstructionWidget::OnToggleDoorClicked);
	}

	if (Wall)
	{
		Wall->OnClicked.AddDynamic(this, &UWallConstructionWidget::SpawnWall);

	}

	if (Slab)
	{
		Slab->OnClicked.AddDynamic(this, &UWallConstructionWidget::SpawnSlab);

	}
	if (LengthInput)
	{
		LengthInput->OnValueChanged.AddDynamic(this, &UWallConstructionWidget::LengthInputChangeHandle);
	}
}


void UWallConstructionWidget::SpawnWall()
{
	if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
	{
		PlayerController->SpawnSelectedActor(EObjectType::Wall);
	}
}

void UWallConstructionWidget::SpawnSlab()
{
	if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
	{
		PlayerController->SpawnSelectedActor(EObjectType::Slab);
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

void UWallConstructionWidget::LengthInputChangeHandle(float InputValue)
{
	if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
	{
		
	}
}
