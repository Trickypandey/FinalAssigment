// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "AssertManager.h"
#include "SAssertWidget.h"
#include "USelectionScrollBox.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMaterialAssetThumbnailSelectedEvent, const FMaterialData&, MeshData);

UCLASS()
class FINALASSIGMENT_API UUSelectionScrollBox : public UWidget
{
	GENERATED_BODY()
protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual const FText GetPaletteCategory() override;

	virtual void SynchronizeProperties() override;

	UFUNCTION()
	void HandleAssetMaterialThumbnailSelected(const FMaterialData& MaterialData);

private:
	TSharedPtr<SAssertWidget> ScrollableAssetBox;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Data")
	UAssertManager* MeshDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Data")
	float ThumbnailSize = 1;

	//UPROPERTY(BlueprintAssignable, Category = "Material Data")
	FMaterialAssetThumbnailSelectedEvent OnMaterialAssetThumbnailSelected;
};
