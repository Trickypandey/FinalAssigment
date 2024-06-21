// Fill out your copyright notice in the Description page of Project Settings.


#include "USelectionScrollBox.h"

TSharedRef<SWidget> UUSelectionScrollBox::RebuildWidget()
{
	Super::RebuildWidget();
	ScrollableAssetBox = SNew(SAssertWidget).InMeshDataAsset(MeshDataAsset).InThumbnailSize(ThumbnailSize);
	ScrollableAssetBox->OnMaterialThumbnailSelected.BindUObject(this, &UUSelectionScrollBox::HandleAssetMaterialThumbnailSelected);
	return ScrollableAssetBox.ToSharedRef();
}

const FText UUSelectionScrollBox::GetPaletteCategory()
{
	return FText::FromString("Panel");
}

void UUSelectionScrollBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UUSelectionScrollBox::HandleAssetMaterialThumbnailSelected(const FMaterialData& MaterialData){
	OnMaterialAssetThumbnailSelected.Broadcast(MaterialData);
}
