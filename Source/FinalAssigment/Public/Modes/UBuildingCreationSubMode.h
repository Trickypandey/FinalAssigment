// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CubeActor.h"
#include "InputMappingContext.h"
#include "WallConstructionWidget.h"
#include "UObject/NoExportTypes.h"
#include "UBuildingCreationSubMode.generated.h"

/**
 * 
 */
UCLASS(ABSTRACT)
class FINALASSIGMENT_API UUBuildingCreationSubMode : public UObject
{
	GENERATED_BODY()
public:

	virtual void Setup() PURE_VIRTUAL(UBuildingCreationSubMode::Setup(), );
	virtual void Cleanup() PURE_VIRTUAL(UBuildingCreationSubMode::Cleanup(), );
	virtual void SetupInputMapping() PURE_VIRTUAL(UBuildingCreationSubMode::SetupInputMapping(), );
	virtual void EnterSubMode(UWallConstructionWidget*) PURE_VIRTUAL(UBuildingCreationSubMode::EnterSubMode, );
	virtual void ExitSubMode(UWallConstructionWidget*) PURE_VIRTUAL(UBuildingCreationSubMode::ExitSubMode, );

	virtual void InitParams(APlayerController* Controller);
	void SetMaterial(UMaterialInterface* Material);

	ACubeActor* GetSelectedActor();

protected:
	FHitResult GetHitResult(const TArray<AActor*>& ActorsToIgnore = TArray<AActor*>{}) const;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UInputMappingContext* InputMappingContext;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY()
	ACubeActor* SelectedActor = nullptr;
	
};
