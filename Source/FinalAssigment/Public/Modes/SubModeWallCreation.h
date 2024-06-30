// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WallActor.h"
#include "WallConstructionWidget.h"
#include "Modes/UBuildingCreationSubMode.h"
#include "SubModeWallCreation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class USubModeWallCreation : public UUBuildingCreationSubMode
{
	GENERATED_BODY()

public:
	USubModeWallCreation();

	virtual void Setup() override;
	virtual void Cleanup() override;
	virtual void SetupInputMapping() override;
	virtual void EnterSubMode(UWallConstructionWidget* Widget) override;
	virtual void ExitSubMode(UWallConstructionWidget* Widget) override;

	void WallRightClickProcess();
	void RotateSelectedActor();
	void WallLeftClickProcess();
	void DeleteSelectedWallActor();

	bool bIsDoorAdding = false;
	UPROPERTY()
	UWallConstructionWidget* CurrentWidget;

private:
	
	UPROPERTY()
	UInputAction* OnWallLeftClick;

	UPROPERTY()
	UInputAction* OnWallRightClick;

	UPROPERTY()
	UInputAction* OnWallRotate;

	UPROPERTY()
	UInputAction* OnWallDelete;

	
};