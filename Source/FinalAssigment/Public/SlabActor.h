// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CubeActor.h"
#include "Utility.h"
#include "SlabActor.generated.h"

/**
 * 
 */
UCLASS()
class FINALASSIGMENT_API ASlabActor : public ACubeActor
{
	GENERATED_BODY()
public:
    ASlabActor();

protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void CreateMesh() override;

    UPROPERTY()
    EBuildingSubModeState WallState;

	
};
