// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UArchVizSaveGame.generated.h"

USTRUCT()
struct FRoadActorData
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FVector> SplinePoints;

    UPROPERTY()
    FTransform ActorTransform;

    UPROPERTY()
    UMaterialInterface* RoadMaterial = nullptr;

};
UCLASS()
class FINALASSIGMENT_API UUArchVizSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FRoadActorData> RoadActorArray;
};
