// Fill out your copyright notice in the Description page of Project Settings.


#include "WallConstructionWidget.h"

#include "ArchVizPlayerController.h"
#include "WallActor.h"



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

	if (Floor)
	{
		Floor->OnClicked.AddDynamic(this, &UWallConstructionWidget::SpawnFloor);

	}
	if (Ceiling)
	{
		Ceiling->OnClicked.AddDynamic(this, &UWallConstructionWidget::SpawnCeiling);
		
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
		PlayerController->BuildingConstructionMode->SpawnSelectedActor(EBuildingCreationType::Wall);
	}
}

void UWallConstructionWidget::SpawnFloor()
{
	if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
	{
		PlayerController->BuildingConstructionMode->SpawnSelectedActor(EBuildingCreationType::Floor);
	}
}

void UWallConstructionWidget::SpawnCeiling()
{
	if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
	{
		PlayerController->BuildingConstructionMode->SpawnSelectedActor(EBuildingCreationType::Ceiling);
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
		
		/*if (auto Actor = PlayerController->BuildingConstructionMode->GetSelectedActor())
		{
			
			Actor->SetLength(InputValue);
			Actor->CreateMesh();

		}*/
	}
}

void UWallConstructionWidget::WidthInputChangeHandle(float InputValue)
{
	if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
	{
		
		/*if (auto Actor = PlayerController->BuildingConstructionMode->GetSelectedActor())
		{

			Actor->SetWidth(InputValue);
			Actor->CreateMesh();

		}*/
		

	}
}

void UWallConstructionWidget::MaterialChangeHandler(const FMaterialData& MeshData)
{
	if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
	{
		PlayerController->BuildingConstructionMode->ApplyMaterialWallProceduralMesh(MeshData);
	}
}
