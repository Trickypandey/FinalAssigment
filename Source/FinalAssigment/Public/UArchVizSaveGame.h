// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteriorDesignActor.h"
#include "GameFramework/SaveGame.h"
#include "UArchVizSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FInteriorActorData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TSubclassOf<AInteriorDesignActor> ActorClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FTransform ActorTransform;

    //UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    //FString AttachedActorName;

    //UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    //FString ActorName;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString StaticMeshPath;
};


USTRUCT(BlueprintType)
struct FBuildingActorData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ActorClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    EBuildingSubModeState WallState;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<bool> bIsDoorAdded ;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FTransform ActorTransform;

    //UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    //FString ActorName; // Unique name for the actor
    UPROPERTY()
    FVector StartLocation;

	UPROPERTY()
    FVector EndLocation;

    UPROPERTY()
    UMaterialInterface* Material = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<FInteriorActorData> AttachedInteriorActors; 
};

USTRUCT(BlueprintType)
struct FSaveGameData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<FBuildingActorData> WallActors;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<FBuildingActorData> FloorActors;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<FBuildingActorData> CeilingActors;
};

USTRUCT()
struct FRoadActorData
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FVector> SplinePoints;

    UPROPERTY()
    FTransform ActorTransform;

    UPROPERTY()
    float Width;

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

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FSaveGameData SaveData;
};
