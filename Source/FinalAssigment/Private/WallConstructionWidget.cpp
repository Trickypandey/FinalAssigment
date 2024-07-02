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

	if (WallMaterialSelectionWidgetInstance)
	{
		WallMaterialSelectionWidgetInstance->OnMaterialAssetThumbnailSelected.AddDynamic(this, &UWallConstructionWidget::MaterialChangeHandler);
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
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Toggle hit"));
	bIsAddingDoor = !bIsAddingDoor;
	if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
	{
		PlayerController->SetIsAddingDoor(bIsAddingDoor);
	}
	if (bIsAddingDoor)
	{
		ToggleDoorButton->SetBackgroundColor(FColor::Black);
	}
	else
	{
		ToggleDoorButton->SetBackgroundColor(FColor::White);
	}
}

void UWallConstructionWidget::LengthInputChangeHandle(float InputValue)
{
	if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
	{
		if (PlayerController && PlayerController->BuildingConstructionMode)
		{
			UBuildingCreationMode* BuildingCreationMode = PlayerController->BuildingConstructionMode;

			if (BuildingCreationMode && BuildingCreationMode->GetSelectedActor())
			{
				ACubeActor* Actor = BuildingCreationMode->GetSelectedActor();
				if (Actor)
				{
					Actor->SetLength(InputValue);
					
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Selected actor is null."));
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No selected actor found or BuildingCreationMode is not valid."));
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("BuildingConstructionMode is not initialized or PlayerController is not valid."));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PlayerController is not valid."));
	}
}

void UWallConstructionWidget::WidthInputChangeHandle(float InputValue)
{
	if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
	{
		if (PlayerController && PlayerController->BuildingConstructionMode)
		{
			UBuildingCreationMode* BuildingCreationMode = PlayerController->BuildingConstructionMode;

			if (BuildingCreationMode && BuildingCreationMode->GetSelectedActor())
			{
				ACubeActor* Actor = BuildingCreationMode->GetSelectedActor();
				if (Actor)
				{
					Actor->SetWidth(InputValue);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Selected actor is null."));
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No selected actor found or BuildingCreationMode is not valid."));
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("BuildingConstructionMode is not initialized or PlayerController is not valid."));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PlayerController is not valid."));
	}

}

void UWallConstructionWidget::MaterialChangeHandler(const FMaterialData& MeshData)
{
	if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
	{
		PlayerController->BuildingConstructionMode->ApplyMaterialWallProceduralMesh(MeshData);
	}
}
