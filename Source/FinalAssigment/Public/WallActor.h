// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWallDoorActor.h"
#include "CubeActor.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "UArchVizSaveGame.h"
#include "Utility.h"
#include "WallActor.generated.h"


UCLASS()
class FINALASSIGMENT_API AWallActor : public AActor
{
	GENERATED_BODY()
	
public:
    AWallActor();

protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

public:
	void HandleConstructingState();
    virtual void Tick(float DeltaTime) override;

 

    void SetWallEndLocation(FVector X);
    void SetWallStartLocation(FVector X);

    UFUNCTION(BlueprintCallable, Category = "Wall")
    void SetIsDoorAdded(bool Flag);

    bool GetDoorFlag();

    UFUNCTION(BlueprintCallable, Category = "Wall")
    void SetDimension(int32 _Length, int32 _Width);


	void SetMaterial(UMaterialInterface* NewMaterial);
	int GetWallIndexFromLocation(FVector Location) const;

	void HandleMovingState();
    void HandlePlacedState();

	void CreateWallSegment();
	void SpawnMesh(FVector SpawnLocation);
    void ReplaceDoorWithWall(AAWallDoorActor* DoorWallActor);
	int GetLength() const
	{
        return Length;
	}

	void AddDoor(const FVector& Vector);
	void ReplaceWallWithDoor(int32 index);
	void ReplaceWallWithDoor(FVector HitLocation);
	void SpawnDoorActor(FVector SpawnLocation);
	FVector GetLocationFromIndex(int32 Index) const;
	void HighlightSelectedActor();
	void UnhighlightDeselectedActor();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
    bool IsDoorAdded;

	UPROPERTY()
    EBuildingCreationType ConstructionType;

    UPROPERTY()
    EBuildingSubModeState WallState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
	int Length;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
	int Width;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
	int Height;
    UPROPERTY()
    int NumberOfSegment;

    UPROPERTY()
	FRotator Rotation;

    UPROPERTY()
	UMaterialInterface* Material;
	TArray<bool> bHasDoorWall;
    


private:
    UPROPERTY()
    TArray<ACubeActor*> WallSegments;

    UPROPERTY()
    TArray<AAWallDoorActor*> DoorWalls;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
    FVector Startlocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
    FVector Endlocation;
};
