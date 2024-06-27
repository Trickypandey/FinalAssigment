// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMode.h"
#include "InteriorDesignMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FINALASSIGMENT_API UInteriorDesignMode : public UBaseMode
{
	GENERATED_BODY()
public:
	UInteriorDesignMode();

	virtual void Setup() override;
	virtual void SetupInputMapping() override;
	virtual void EnterMode() override;
	virtual void ExitMode() override;

private:
	UFUNCTION()
	void HandleLeftClickAction();
	
};
