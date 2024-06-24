// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "USelectionScrollBox.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/SpinBox.h"
#include "RoadCreationWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALASSIGMENT_API URoadCreationWidget : public UUserWidget
{
	GENERATED_BODY()
		
public:
	UFUNCTION()
	void MaterialChangeHandler(const FMaterialData& MeshData);
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas;

	// Input fields for wall dimensions
	UPROPERTY(meta = (BindWidget))
	USpinBox* HeightInput;

	UPROPERTY(meta = (BindWidget))
	USpinBox* WidthInput;

	UPROPERTY(meta = (BindWidget))
	UUSelectionScrollBox* SelectionWidgetInstance;
	
};
