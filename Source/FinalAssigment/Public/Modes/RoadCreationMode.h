// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssertManager.h"
#include "BaseMode.h"
#include "InputAction.h"
#include "RoadActor.h"
#include "RoadCreationMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FINALASSIGMENT_API URoadCreationMode : public UBaseMode
{
	GENERATED_BODY()

public:
	URoadCreationMode();
	virtual void Setup() override;
	;
	virtual void SetupInputMapping() override;
	virtual void EnterMode() override;
	virtual void ExitMode() override;
	virtual void CleanUp() override;
	void AddMaterialToRoad(const FMaterialData& MeshData);
	void SaveRoads(UUArchVizSaveGame*& SaveGameInstance);
	void LoadRoads(UUArchVizSaveGame*& LoadGameInstance);
	void SetWidth(float Invalue);

private:



	UFUNCTION()
	void HandleLeftClickAction();

	UFUNCTION()
	void HandleRightClickAction();

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "RoadConstrcution | Actor")
	ARoadActor* CurrentRoadActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RoadConstrcution | Actor", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARoadActor> RoadActorRef;
	
	UPROPERTY()
	UInputAction* OnLeftClickAction;

	UPROPERTY()
	UInputAction* OnRoadRightClick;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY()
	TArray<ARoadActor*> RoadActors;
	
};
