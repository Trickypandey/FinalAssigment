// Fill out your copyright notice in the Description page of Project Settings.


#include "SlabActor.h"

ASlabActor::ASlabActor()
{
    PrimaryActorTick.bCanEverTick = true;
    Length = 400.0f;
    Width = 400.0f;
    Height = 10.0f; // Typical slab thickness
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

void ASlabActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASlabActor::CreateMesh()
{
    ResetArrays();

    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> VertexColors;

    // Define vertices with the origin at one corner (bottom-left-front corner)
    Vertices.Add(FVector(0.0f, 0.0f, 0.0f));
    Vertices.Add(FVector(Length, 0.0f, 0.0f));
    Vertices.Add(FVector(Length, Width, 0.0f));
    Vertices.Add(FVector(0.0f, Width, 0.0f));

    Vertices.Add(FVector(0.0f, 0.0f, Height));
    Vertices.Add(FVector(Length, 0.0f, Height));
    Vertices.Add(FVector(Length, Width, Height));
    Vertices.Add(FVector(0.0f, Width, Height));

    // Define triangles (two for each face, 12 in total)
    Triangles = {
        0, 1, 2, 0, 2, 3,  // Bottom face
        4, 6, 5, 4, 7, 6,  // Top face
        0, 4, 1, 1, 4, 5,  // Front face
        1, 5, 2, 2, 5, 6,  // Right face
        2, 6, 3, 3, 6, 7,  // Back face
        3, 7, 0, 0, 7, 4   // Left face
    };

    // Define normals for each vertex
    Normals.Add(FVector(0.0f, 0.0f, -1.0f));
    Normals.Add(FVector(0.0f, 0.0f, -1.0f));
    Normals.Add(FVector(0.0f, 0.0f, -1.0f));
    Normals.Add(FVector(0.0f, 0.0f, -1.0f));

    Normals.Add(FVector(0.0f, 0.0f, 1.0f));
    Normals.Add(FVector(0.0f, 0.0f, 1.0f));
    Normals.Add(FVector(0.0f, 0.0f, 1.0f));
    Normals.Add(FVector(0.0f, 0.0f, 1.0f));

    Normals.Add(FVector(0.0f, -1.0f, 0.0f));
    Normals.Add(FVector(0.0f, -1.0f, 0.0f));
    Normals.Add(FVector(0.0f, -1.0f, 0.0f));
    Normals.Add(FVector(0.0f, -1.0f, 0.0f));

    Normals.Add(FVector(1.0f, 0.0f, 0.0f));
    Normals.Add(FVector(1.0f, 0.0f, 0.0f));
    Normals.Add(FVector(1.0f, 0.0f, 0.0f));
    Normals.Add(FVector(1.0f, 0.0f, 0.0f));

    Normals.Add(FVector(0.0f, 1.0f, 0.0f));
    Normals.Add(FVector(0.0f, 1.0f, 0.0f));
    Normals.Add(FVector(0.0f, 1.0f, 0.0f));
    Normals.Add(FVector(0.0f, 1.0f, 0.0f));

    Normals.Add(FVector(-1.0f, 0.0f, 0.0f));
    Normals.Add(FVector(-1.0f, 0.0f, 0.0f));
    Normals.Add(FVector(-1.0f, 0.0f, 0.0f));
    Normals.Add(FVector(-1.0f, 0.0f, 0.0f));

    // Define UVs
    UVs.Add(FVector2D(0.0f, 0.0f));
    UVs.Add(FVector2D(1.0f, 0.0f));
    UVs.Add(FVector2D(1.0f, 1.0f));
    UVs.Add(FVector2D(0.0f, 1.0f));

    UVs.Add(FVector2D(0.0f, 0.0f));
    UVs.Add(FVector2D(1.0f, 0.0f));
    UVs.Add(FVector2D(1.0f, 1.0f));
    UVs.Add(FVector2D(0.0f, 1.0f));

    // Define tangents
    Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
    Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
    Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
    Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));

    Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
    Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
    Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
    Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));

    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
}
