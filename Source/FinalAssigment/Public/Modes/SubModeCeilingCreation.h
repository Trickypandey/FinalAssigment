// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modes/UBuildingCreationSubMode.h"
#include "SubModeCeilingCreation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FINALASSIGMENT_API USubModeCeilingCreation : public UUBuildingCreationSubMode
{
	GENERATED_BODY()
public:

	USubModeCeilingCreation();

	virtual void Setup() override;
	virtual void Cleanup() override;
	void RotateSelectedActor();
	virtual void SetupInputMapping() override;
	virtual void EnterSubMode(UWallConstructionWidget*) override;
	virtual void ExitSubMode(UWallConstructionWidget*) override;
	void ShowInstructionTab();
	void ToggleMovementSelectedActor();
	void HideInstructionTab();

	void WallLeftClickProcess();
	void WallRightClickProcess();
	void DeleteSelectedWallActor();

private:
	UPROPERTY()
	UInputAction* OnWallLeftClick;

	UPROPERTY()
	UInputAction* OnWallRightClick;

	UPROPERTY()
	UInputAction* OnWallDelete;

	UPROPERTY()
	UWallConstructionWidget* CurrentWidget;
	UPROPERTY()
	UInputAction* OnShowInstruction;

	UPROPERTY()
	UInputAction* StartMovement;

	UPROPERTY()
	UInputAction* OnActorRotate;
	bool bFirstClickDone;
	bool bIsNewWall = false;
};
