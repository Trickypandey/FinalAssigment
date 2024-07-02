// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "USelectionScrollBox.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
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
	UFUNCTION()
	void WidthInputChangeHandle(float InValue);
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas;

	UPROPERTY(meta = (BindWidget))
	USpinBox* WidthInput;

	UPROPERTY(meta = (BindWidget))
	UUSelectionScrollBox* SelectionWidgetInstance;

	UPROPERTY(meta = (BindWidget))
	UBorder* InstructionBtn;

	UPROPERTY(meta = (BindWidget))
	UBorder* Allkeys;
	
};
