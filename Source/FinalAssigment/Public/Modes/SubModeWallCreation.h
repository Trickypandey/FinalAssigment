// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WallActor.h"
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
	virtual void EnterSubMode() override;
	virtual void ExitSubMode() override;

	void WallRightClickProcess();
	void RotateSelectedActor();
	void WallLeftClickProcess();
	void DeleteSelectedWallActor();


private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wall" , meta =(AllowPrivateAccess))
	TSubclassOf<AWallActor> WallActorRef;

	UPROPERTY()
	UInputAction* OnWallLeftClick;

	UPROPERTY()
	UInputAction* OnWallRightClick;

	UPROPERTY()
	UInputAction* OnWallRotate;

	UPROPERTY()
	UInputAction* OnWallDelete;
	
};
