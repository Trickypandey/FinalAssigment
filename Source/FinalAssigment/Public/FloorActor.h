// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlabActor.h"
#include "Utility.h"
#include "FloorActor.generated.h"

/**
 * 
 */
UCLASS()
class FINALASSIGMENT_API AFloorActor : public ASlabActor
{
	GENERATED_BODY()
public:
	//virtual void Tick(float DeltaSeconds) override;
	AFloorActor();
	UPROPERTY()
	EBuildingCreationType ConstructionType;

	
};
