// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadCreationWidget.h"

#include "ArchVizPlayerController.h"


void URoadCreationWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (SelectionWidgetInstance)
	{
		SelectionWidgetInstance->OnMaterialAssetThumbnailSelected.AddDynamic(this, &URoadCreationWidget::MaterialChangeHandler);
	}
}

void URoadCreationWidget::MaterialChangeHandler(const FMaterialData& MeshData)
{
	if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
	{

		PlayerController->RoadConstructionMode->AddMaterialToRoad(MeshData);
	}
}