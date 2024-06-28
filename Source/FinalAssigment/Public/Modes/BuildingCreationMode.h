// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssertManager.h"
#include "BaseMode.h"
#include "CubeActor.h"
#include "InputAction.h"
#include "SubModeCeilingCreation.h"
#include "SubModeFloorCreation.h"
#include "SubModeWallCreation.h"
#include "Utility.h"
#include "BuildingCreationMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FINALASSIGMENT_API UBuildingCreationMode : public UBaseMode
{
	GENERATED_BODY()
public:
	UBuildingCreationMode();
	virtual void Setup() override;
	void SetSubMode(UUBuildingCreationSubMode* NewSubMode);
	;
	
	//TPair<ACubeActor*, FVector> IsWallActor(const FHitResult& HitResult);
	
	
	//void DeSelectedSelectedActor();
	void SpawnSelectedActor(EBuildingCreationType Type);
	void ApplyMaterialWallProceduralMesh(const FMaterialData& MeshData);
	/*bool bIsActorSpawning;
	bool bIsAddingDoor;
	bool IsSlapOrFloor = false;*/
	virtual void SetupInputMapping() override;
	virtual void EnterMode() override;
	virtual void ExitMode() override;
	virtual void CleanUp() override;
	ACubeActor* GetSelectedActor();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BuildingCreationMode | Wall")
	TSubclassOf<USubModeWallCreation> WallCreationModeRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BuildingCreationMode | Floor")
	TSubclassOf<USubModeFloorCreation> FloorCreationModeRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BuildingCreationMode | Roof")
	TSubclassOf<USubModeCeilingCreation> CeilingModeRef;

	UPROPERTY(VisibleDefaultsOnly, Category = "BuildingCreationMode | Wall")
	USubModeWallCreation* WallCreationMode;

private:
	UUBuildingCreationSubMode* CurrentBuildingCreationSubMode;

	UPROPERTY()
	EBuildingCreationType BuildingModeEntity;


	UPROPERTY(VisibleDefaultsOnly, Category = "BuildingCreationMode | Floor")
	USubModeFloorCreation* FloorCreationMode;

	UPROPERTY(VisibleDefaultsOnly, Category = "BuildingCreationMode | Roof")
	USubModeCeilingCreation* CeilingCreationMode;

	//FVector OriginalLocation;

	//UPROPERTY()
	//UInputAction* OnWallLeftClick;

	//UPROPERTY()
	//UInputAction* OnWallRightClick;

	//UPROPERTY()
	//UInputAction* OnWallRotate;

	//UPROPERTY()
	//UInputAction* OnWallDelete;

	//UPROPERTY()
	//UInputAction* OnDeSelectWall;

	//UPROPERTY()
	//ACubeActor* SelectedActor;

	//UPROPERTY()
	//ACubeActor* PreviousSelectedActor;

	//UPROPERTY()
	//AActor* HoveringActor;

	EBuildingCreationType SelectedActorType;
};
