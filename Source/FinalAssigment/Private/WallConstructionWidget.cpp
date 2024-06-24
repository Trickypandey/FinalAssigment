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

	if (WidthInput)
	{
		WidthInput->OnValueChanged.AddDynamic(this,&UWallConstructionWidget::WidthInputChangeHandle );
	}

	if (SelectionWidgetInstance)
	{
		SelectionWidgetInstance->OnMaterialAssetThumbnailSelected.AddDynamic(this, &UWallConstructionWidget::MaterialChangeHandler);
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
		// there is a case where it could be  null if occur go for pointer  int getselected actor
		if (auto Actor = PlayerController->GetSelectedActor())
		{
			
			Actor->SetLength(InputValue);
			Actor->CreateMesh();

		}
	}
}

void UWallConstructionWidget::WidthInputChangeHandle(float InputValue)
{
	if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
	{
		// there is a case where it could be  null if occur go for pointer  int getselected actor
		if (auto Actor = PlayerController->GetSelectedActor())
		{

			Actor->SetWidth(InputValue);
			Actor->CreateMesh();

		}
		

	}
}

void UWallConstructionWidget::MaterialChangeHandler(const FMaterialData& MeshData)
{
	if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
	{
		PlayerController->ApplyMaterialWallProceduralMesh(MeshData);
	}
}
