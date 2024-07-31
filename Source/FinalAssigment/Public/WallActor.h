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

	FVector GetWallEndLocation();
    FVector GetWallStartLocation();

    UFUNCTION(BlueprintCallable, Category = "Wall")
    void SetIsDoorAdded(bool Flag);

    bool GetDoorFlag();

    UFUNCTION(BlueprintCallable, Category = "Wall")
    void SetDimension(int32 _Length, int32 _Width);


	void SetMaterial(UMaterialInterface* NewMaterial);
	void HandleMovingState();
    void HandlePlacedState();
	void CreateWallSegment();
	void SpawnMesh(FVector SpawnLocation);
    void ReplaceDoorWithWall();
	int GetLength() const
	{
        return Length;
	}

	void AddDoor(ACubeActor*& actor);
	void HighlightSelectedActor();
	void UnhighlightDeselectedActor();
	void RotateWall(float Angle);
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

    UPROPERTY()
    AAWallDoorActor* currentDoorActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
    TArray<ACubeActor*> WallSegments;
private:
    

    UPROPERTY()
    TArray<AAWallDoorActor*> DoorWalls;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
    FVector Startlocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
    FVector Endlocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
    USceneComponent* SceneComponent;
};
