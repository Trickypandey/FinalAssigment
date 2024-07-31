// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlabActor.h"
#include "Utility.h"
#include "CeilingActor.generated.h"

/**
 * 
 */
UCLASS()
class FINALASSIGMENT_API ACeilingActor : public ASlabActor
{
	GENERATED_BODY()

public:

	ACeilingActor();
	UPROPERTY()
	EBuildingCreationType ConstructionType;
	
};
