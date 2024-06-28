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
	virtual void SetupInputMapping() override;
	virtual void EnterSubMode(UWallConstructionWidget*) override;
	virtual void ExitSubMode(UWallConstructionWidget*) override;

	void WallLeftClickProcess();
	void WallRightClickProcess();

private:
	UPROPERTY()
	UInputAction* OnWallLeftClick;

	UPROPERTY()
	UInputAction* OnWallRightClick;

	UPROPERTY()
	UWallConstructionWidget* CurrentWidget;
};
