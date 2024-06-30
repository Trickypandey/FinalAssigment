// Fill out your copyright notice in the Description page of Project Settings.


#include "InteriorDesignWidget.h"

#include "ArchVizPlayerController.h"


void UInteriorDesignWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (FurnitureScrollBoxInstance)
	{
		FurnitureScrollBoxInstance->OnFurnitureAssetThumbnailSelected.AddDynamic(this, &UInteriorDesignWidget::HandleFurnitureAdding);
	}
}
void UInteriorDesignWidget::HandleFurnitureAdding(const FFurnitureData& MeshData)
{
	if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
	{

		PlayerController->InteriorDesignMode->SetMeshData(MeshData) ;
	}
}
