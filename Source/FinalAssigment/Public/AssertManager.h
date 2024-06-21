// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AssertManager.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMaterialData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UMaterialInterface* Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UTexture2D* Icon;

};
UCLASS()
class FINALASSIGMENT_API UAssertManager : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	TArray<FMaterialData> MaterialDataArray;
};
