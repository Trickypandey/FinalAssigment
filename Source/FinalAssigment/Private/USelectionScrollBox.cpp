// Fill out your copyright notice in the Description page of Project Settings.


#include "USelectionScrollBox.h"



TSharedRef<SWidget> UUSelectionScrollBox::RebuildWidget()
{
	Super::RebuildWidget();
	ScrollableAssetBox = SNew(SAssertWidget).InMeshDataAsset(MeshDataAsset).InThumbnailSize(ThumbnailSize).InAssetType(AssetType);
	ScrollableAssetBox->OnMaterialThumbnailSelected.BindUObject(this, &UUSelectionScrollBox::HandleAssetMaterialThumbnailSelected);
	ScrollableAssetBox->OnFurnitureThumbnailSelected.BindUObject(this, &UUSelectionScrollBox::HandleAssetFurnitureThumbnailSelected);
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
void UUSelectionScrollBox::HandleAssetFurnitureThumbnailSelected(const FFurnitureData& FurnitureData)
{
	OnFurnitureAssetThumbnailSelected.Broadcast(FurnitureData);
}


void UUSelectionScrollBox::ReleaseSlateResources(bool bReleaseChildren) {

	Super::ReleaseSlateResources(bReleaseChildren);

	ScrollableAssetBox.Reset();

}

