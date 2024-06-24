// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadCreationWidget.h"


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
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Road Material CHange"));
}