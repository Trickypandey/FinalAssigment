// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "USelectionScrollBox.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "InteriorDesignWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALASSIGMENT_API UInteriorDesignWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void HandleFurnitureAdding(const FFurnitureData& MeshData);
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UUSelectionScrollBox* FurnitureScrollBoxInstance;

	UPROPERTY(meta = (BindWidget))
	UBorder* InstructionBtn;

	UPROPERTY(meta = (BindWidget))
	UBorder* Allkeys;
	
};
