// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALASSIGMENT_API USaveGameWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* SaveNameInput;

	UPROPERTY(meta = (BindWidget))
	class UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	UFUNCTION()
	void OnSaveButtonClicked();

	UFUNCTION()
	void OnCancelButtonClicked();
	
};
