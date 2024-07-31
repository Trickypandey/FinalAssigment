// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WallConstructionWidget.h"
#include "Modes/UBuildingCreationSubMode.h"
#include "SubModeFloorCreation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FINALASSIGMENT_API USubModeFloorCreation : public UUBuildingCreationSubMode
{
	GENERATED_BODY()
public:
	USubModeFloorCreation();

	virtual void Setup() override;
	virtual void Cleanup() override;
	void ToggleMovementSelectedActor();
	void DeleteSelectedActor();
	void RotateSelectedActor();
	virtual void SetupInputMapping() override;
	virtual void EnterSubMode(UWallConstructionWidget*) override;
	virtual void ExitSubMode(UWallConstructionWidget*) override;
	void ShowInstructionTab();
	void HideInstructionTab();
	void WallLeftClickProcess();
	void WallRightClickProcess();

private:
	UPROPERTY()
	UInputAction* OnWallLeftClick;

	UPROPERTY()
	UInputAction* OnWallRightClick;
	UPROPERTY()
	UWallConstructionWidget* CurrentWidget;

	UPROPERTY()
	UInputAction* OnWallDelete;

	UPROPERTY()
	UInputAction* OnShowInstruction;

	UPROPERTY()
	UInputAction* StartMovement;

	UPROPERTY()
	UInputAction* OnActorRotate;

	bool bFirstClickDone = false;
	bool bIsNewWall = false;
};
