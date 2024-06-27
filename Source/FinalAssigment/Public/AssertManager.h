// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility.h"
#include "Engine/DataAsset.h"
#include "AssertManager.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAssetType : uint8 {
	Material,
	Interior
};


USTRUCT(BlueprintType)
struct FMaterialData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInterface* Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UTexture2D* Icon;

};


USTRUCT(BlueprintType)
struct FFurnitureData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	EBuildingAttachable Attachable;
};

UCLASS()
class FINALASSIGMENT_API UAssertManager : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	TArray<FMaterialData> MaterialDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<FFurnitureData> MeshDataArray;
};
