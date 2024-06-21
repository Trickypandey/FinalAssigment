// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssertManager.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SScrollBox.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FAssetMaterialThumbnailSelected, const FMaterialData&);
class FINALASSIGMENT_API SAssertWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SAssertWidget) {}
        SLATE_ARGUMENT(TWeakObjectPtr<UAssertManager>, InMeshDataAsset)
        SLATE_ARGUMENT(float, InThumbnailSize)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    TSharedPtr<SScrollBox> ScrollBox;
    TWeakObjectPtr<UAssertManager> MeshDataAsset;
    float ThumbnailSize;

    void RefreshContent() const;
    void DisplayMaterials(const TArray<FMaterialData>& DataArray) const;

public:
    FAssetMaterialThumbnailSelected OnMaterialThumbnailSelected;

};
