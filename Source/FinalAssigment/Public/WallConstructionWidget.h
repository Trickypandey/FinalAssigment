// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/EditableTextBox.h"
#include "Utility.h"
#include "Components/SpinBox.h"
#include "WallConstructionWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALASSIGMENT_API UWallConstructionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas;

	// Button to toggle add door flag
	UPROPERTY(meta = (BindWidget))
	UButton* ToggleDoorButton;

	// Input fields for wall dimensions
	UPROPERTY(meta = (BindWidget))
	USpinBox* LengthInput;

	UPROPERTY(meta = (BindWidget))
	USpinBox* WidthInput;

	UPROPERTY(meta = (BindWidget))
	USpinBox* HeightInput;

	UPROPERTY(meta = (BindWidget))
	UButton* Wall;

	UPROPERTY(meta = (BindWidget))
	UButton* Slab;

	UFUNCTION()
	void SpawnWall();

	UFUNCTION()
	void SpawnSlab();

	UFUNCTION()
	void OnToggleDoorClicked();

	UFUNCTION()
	void LengthInputChangeHandle(float InputValue);

	private:
	bool bIsAddingDoor = false;

	
};
