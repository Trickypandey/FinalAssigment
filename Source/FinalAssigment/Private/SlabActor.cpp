// Fill out your copyright notice in the Description page of Project Settings.


#include "SlabActor.h"

ASlabActor::ASlabActor()
{
    PrimaryActorTick.bCanEverTick = true;
    Length = 400.0f;
    Width = 400.0f;
    Height = 20.0f;
    WallState = EBuildingSubModeState::Placed;
}

void ASlabActor::BeginPlay()
{
    Super::BeginPlay();
    CreateMesh();
}

void ASlabActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    CreateMesh();
}


void ASlabActor::CreateMesh()
{

    ProceduralMesh->ClearAllMeshSections();
    ResetArrays();

    float L = Length;
    float W = Width;
    float H = Height;

    // Adjusted positions with the origin (0, 0, 0) at the bottom-left corner
    Vertices.Add(FVector(0, 0, 0));         // 0
    Vertices.Add(FVector(0, W, 0));         // 1
    Vertices.Add(FVector(0, W, H));         // 2
    Vertices.Add(FVector(0, 0, H));         // 3

    Vertices.Add(FVector(0, 0, 0));         // 4
    Vertices.Add(FVector(L, 0, 0));         // 5
    Vertices.Add(FVector(L, W, 0));         // 6
    Vertices.Add(FVector(0, W, 0));         // 7

    Vertices.Add(FVector(L, 0, 0));         // 8
    Vertices.Add(FVector(L, 0, H));         // 9
    Vertices.Add(FVector(L, W, H));         // 10
    Vertices.Add(FVector(L, W, 0));         // 11

    Vertices.Add(FVector(L, 0, H));         // 12
    Vertices.Add(FVector(0, 0, H));         // 13
    Vertices.Add(FVector(0, W, H));         // 14
    Vertices.Add(FVector(L, W, H));         // 15

    Vertices.Add(FVector(0, 0, H));         // 16
    Vertices.Add(FVector(L, 0, H));         // 17
    Vertices.Add(FVector(L, 0, 0));         // 18
    Vertices.Add(FVector(0, 0, 0));         // 19

    Vertices.Add(FVector(0, W, H));         // 20
    Vertices.Add(FVector(0, W, 0));         // 21
    Vertices.Add(FVector(L, W, 0));         // 22
    Vertices.Add(FVector(L, W, H));         // 23

    Triangles = {
         0, 1, 3, 1, 2, 3,
         4, 5, 7, 5, 6, 7,
         8, 9, 11, 9, 10, 11,
         12, 13, 15, 13, 14, 15,
         16, 17, 19, 17, 18, 19,
         20, 21, 23, 21, 22, 23
    };

    AddNormals();
    AddUVs();
    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
}
