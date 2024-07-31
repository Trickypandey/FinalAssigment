// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CubeActor.h"
#include "Utility.h"
#include "CubeActor.h"
#include "SlabActor.generated.h"

/**
 * 
 */
UCLASS()
class FINALASSIGMENT_API ASlabActor : public AActor
{
	GENERATED_BODY()
public:
    ASlabActor();
    void SetEndLocation(FVector X);
    void SetStartLocation(FVector X);

protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

public:
	void CreateMesh() ;
	void GenerateCube(int Segment);
	void GenerateSlab();
	void PlaceSlab(FVector NewLocation);
	void Tick(float DeltaSeconds);
	void HandlePlacedState();
	void HandleConstructingState();
	void HandleMovingState();
	void BuildQuad(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FProcMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector BottomLeft, const FVector BottomRight, const FVector TopRight, const FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, const FVector Normal, const FProcMeshTangent Tangent);
    void AddTriangle(TArray<int32>& Triangles, int32 Index1, int32 Index2, int32 Index3);
    void SetMaterial(UMaterialInterface* NewMaterial);
    void HighlightSelectedActor();
    void UnhighlightDeselectedActor();

    UPROPERTY()
    UMaterialInterface* Material;

	UPROPERTY()
    EBuildingSubModeState WallState;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "SlabProp")
    FVector StartLocation;
 
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "SlabProp")
    FVector EndLocation;
   
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "SlabProp")
	float Length;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "SlabProp")
	float Height;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "SlabProp")
	float Width;

    UPROPERTY()
    ACubeActor* Slab;

    UPROPERTY()
	UProceduralMeshComponent* ProceduralSlabMesh;
};
