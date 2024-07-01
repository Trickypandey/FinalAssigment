// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "ToastMessageWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FToastMessageDelegate, const FString&, Message);

UCLASS()
class FINALASSIGMENT_API UToastMessageWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FToastMessageDelegate ShowMessage;
protected:
	virtual void NativeConstruct() override;
private:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* PopUpAnimation;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ToastText;

	UFUNCTION()
	void OnShowToastMessage(const FString& Message);

};
