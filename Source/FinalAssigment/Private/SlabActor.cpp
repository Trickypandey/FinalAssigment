// Fill out your copyright notice in the Description page of Project Settings.


#include "SlabActor.h"

#include "Kismet/KismetMathLibrary.h"

ASlabActor::ASlabActor()
{
    PrimaryActorTick.bCanEverTick = true;
    Length = 100.0f;
    Width = 100.0f;
    Height = 20.0f;
    WallState = EBuildingSubModeState::Moving;
    ProceduralSlabMesh = CreateDefaultSubobject<UProceduralMeshComponent>(FName("ProceduralSlabMesh"));
    RootComponent = ProceduralSlabMesh;
}

void ASlabActor::SetEndLocation(FVector X)
{
    EndLocation = Utility::SnapToGrid(X, FVector(10,10,0));
    CreateMesh();
}

void ASlabActor::SetStartLocation(FVector X)
{
    StartLocation = Utility::SnapToGrid(X, FVector(10,10,0));
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

    // Calculate distances between start and end points
    float XDistance = EndLocation.X - StartLocation.X;
    float YDistance = EndLocation.Y - StartLocation.Y;

    // Calculate dimensions based on absolute values of distances
    Length = FMath::Abs(XDistance);
    Width = FMath::Abs(YDistance);

    // Determine the correct rotation and dimension adjustment
    if (XDistance >= 0.0f && YDistance >= 0.0f) {
        SetActorRotation(FRotator(0, 0, 0));
    }
    else if (XDistance >= 0.0f && YDistance < 0.0f) {
        SetActorRotation(FRotator(0, 270, 0));
        std::swap(Length, Width);
    }
    else if (XDistance < 0.0f && YDistance >= 0.0f) {
        SetActorRotation(FRotator(0, 90, 0));
        std::swap(Length, Width);
    }
    else {
        SetActorRotation(FRotator(0, 180, 0));
    }

    // Calculate the spawn location based on the adjusted StartPoint
    FVector SpawnLocation = StartLocation;
    SpawnLocation.Z = StartLocation.Z;

    // Ensure ProceduralSlabMesh is valid before using it
    if (ProceduralSlabMesh)
    {
        // Clear any previous mesh data
        ProceduralSlabMesh->ClearAllMeshSections();

        // Generate the slab mesh
        GenerateSlab();

        // Set the location of the mesh
        ProceduralSlabMesh->SetWorldLocation(SpawnLocation);
    }
}


void ASlabActor::BuildQuad(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FProcMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector BottomLeft, const FVector BottomRight, const FVector TopRight, const FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, const FVector Normal, const FProcMeshTangent Tangent)
{
    const int32 Index1 = VertexOffset++;
    const int32 Index2 = VertexOffset++;
    const int32 Index3 = VertexOffset++;
    const int32 Index4 = VertexOffset++;
    InVertices[Index1] = BottomLeft;
    InVertices[Index2] = BottomRight;
    InVertices[Index3] = TopRight;
    InVertices[Index4] = TopLeft;
    InTexCoords[Index1] = FVector2D(0.0f, 1.0f);
    InTexCoords[Index2] = FVector2D(1.0f, 1.0f);
    InTexCoords[Index3] = FVector2D(1.0f, 0.0f);
    InTexCoords[Index4] = FVector2D(0.0f, 0.0f);
    InTriangles[TriangleOffset++] = Index1;
    InTriangles[TriangleOffset++] = Index2;
    InTriangles[TriangleOffset++] = Index3;
    InTriangles[TriangleOffset++] = Index1;
    InTriangles[TriangleOffset++] = Index3;
    InTriangles[TriangleOffset++] = Index4;
    // On a cube side, all the vertex normals face the same way
    InNormals[Index1] = InNormals[Index2] = InNormals[Index3] = InNormals[Index4] = Normal;
    InTangents[Index1] = InTangents[Index2] = InTangents[Index3] = InTangents[Index4] = Tangent;
}




void ASlabActor::GenerateCube(int Segment)
{


    float x = Length;
    float y = Width;
    float z = Height;

    TArray<FVector> Vertices;
    TArray<FVector> Normals;
    TArray<FProcMeshTangent> Tangents;
    TArray<FVector2D> UVs;
    TArray<FColor> Colors;
    TArray<int32> Triangles;

    Vertices.Add( FVector(0, y, 0));    // 0
    Vertices.Add( FVector(x, y, 0));    // 1
    Vertices.Add( FVector(x, y, z));    // 2
    Vertices.Add( FVector(0, y, z));    // 3
   Vertices.Add( FVector(x, y, 0));    // 4
    Vertices.Add( FVector(x, 0, 0));    // 5
    Vertices.Add( FVector(x, 0, z));    // 6
    Vertices.Add( FVector(x, y, z));    // 7
   Vertices.Add( FVector(0, y, z));    // 8
    Vertices.Add( FVector(x, y, z));    // 9
    Vertices.Add( FVector(x, 0, z));    // 10
    Vertices.Add( FVector(0, 0, z));    // 11
   Vertices.Add( FVector(0, 0, 0));    // 12
    Vertices.Add( FVector(0, y, 0));    // 13
    Vertices.Add( FVector(0, y, z));    // 14
    Vertices.Add( FVector(0, 0, z));    // 15
   Vertices.Add( FVector(x, 0, 0));    // 16
    Vertices.Add( FVector(0, 0, 0));    // 17
    Vertices.Add( FVector(0, 0, z));    // 18
    Vertices.Add( FVector(x, 0, z));    // 19
   Vertices.Add( FVector(0, 0, 0));    // 20
    Vertices.Add( FVector(x, 0, 0));    // 21
    Vertices.Add( FVector(x, y, 0));    // 22
    Vertices.Add( FVector(0, y, 0));    // 23

    /*Vertices.Add(FVector(-x, y, -z)); Vertices.Add(FVector(x, y, -z)); Vertices.Add(FVector(x, y, z)); Vertices.Add(FVector(-x, y, z));
    Vertices.Add(FVector(x, y, -z)); Vertices.Add(FVector(x, -y, -z)); Vertices.Add(FVector(x, -y, z)); Vertices.Add(FVector(x, y, z));
    Vertices.Add(FVector(-x, y, z)); Vertices.Add(FVector(x, y, z)); Vertices.Add(FVector(x, -y, z)); Vertices.Add(FVector(-x, -y, z));
    Vertices.Add(FVector(-x, -y, -z)); Vertices.Add(FVector(-x, y, -z)); Vertices.Add(FVector(-x, y, z)); Vertices.Add(FVector(-x, -y, z));
    Vertices.Add(FVector(x, -y, -z)); Vertices.Add(FVector(-x, -y, -z)); Vertices.Add(FVector(-x, -y, z)); Vertices.Add(FVector(x, -y, z));
    Vertices.Add(FVector(-x, -y, -z)); Vertices.Add(FVector(x, -y, -z)); Vertices.Add(FVector(x, y, -z)); Vertices.Add(FVector(-x, y, -z));*/

    /* Triangles.Append
     ({
         0,1,2, 0,2,3,
         4,5,6, 7,4,6,
         8,9,10, 11,8,10,
         12,13,14, 15,12,14,
         16,17,18, 19,16,18,
         20,21,22, 23,20,22
         }


     );

     for (int i = 0; i < Triangles.Num(); i += 3)
     {
         FVector temp = (Vertices[Triangles[i + 2]] - Vertices[Triangles[i]]) ^ (Vertices[Triangles[i + 1]] - Vertices[Triangles[i + 2]]);
         Normals.Add(temp);
         Normals.Add(temp);
     }*/

     //Normals = {
     //FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), // Top face
     //FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0), // Right face
     //FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0), // Front face
     //FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0), // Left face
     //FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), // Bottom face
     //FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0)  // Back face
     //};

     // Define the UVs for each vertex
     //UVs = {
     //    FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0), // Top face
     //    FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0),
     //    FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0),
     //    FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0),
     //    FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0),
     //    FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0), // Back face
     //};

     // Define the tangents for each face
    Tangents = {
        FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), // Top face
        FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0), // Right face
        FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), // Front face
        FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0), // Left face
        FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), // Bottom face
        FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0)  // Back face
    };


    for (int32 i = 0; i < 24; i += 4) {
        AddTriangle(Triangles, i, i + 1, i + 2);
        AddTriangle(Triangles, i + 2, i + 3, i);
    }

    //Normals


    for (int32 i = 0; i < 24; i += 4) {
        FVector CurrentNormal = FVector::CrossProduct(UKismetMathLibrary::GetDirectionUnitVector(Vertices[i + 2], Vertices[i + 1]), UKismetMathLibrary::GetDirectionUnitVector(Vertices[i + 1], Vertices[i]));

        Normals.Add(CurrentNormal);
        Normals.Add(CurrentNormal);
        Normals.Add(CurrentNormal);
        Normals.Add(CurrentNormal);
    }

    //UVs


    for (int32 i = 0; i < 24; i += 4) {
        UVs.Add(FVector2D{ 0.0,0.0 });
        UVs.Add(FVector2D{ 0.0,1.0 });
        UVs.Add(FVector2D{ 1.0,1.0 });
        UVs.Add(FVector2D{ 1.0,0.0 });
    }
    // Optionally, add vertex colors
    for (int32 i = 0; i < Vertices.Num(); ++i)
    {
        Colors.Add(FColor::White);
    }

    if (Vertices.Num() == Tangents.Num() && UVs.Num() == Colors.Num() && Normals.Num() == Vertices.Num())
    {
        ProceduralSlabMesh->CreateMeshSection(Segment, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
    }


}
void ASlabActor::AddTriangle(TArray<int32>& Triangles, int32 Index1, int32 Index2, int32 Index3) {
    Triangles.Add(Index1);
    Triangles.Add(Index2);
    Triangles.Add(Index3);
}

void ASlabActor::SetMaterial(UMaterialInterface* NewMaterial)
{
    if (NewMaterial)
    {
        Material = NewMaterial;
        if(ProceduralSlabMesh)
        {
            ProceduralSlabMesh->SetMaterial(0, NewMaterial);
        }
    }
}

void ASlabActor::HighlightSelectedActor()
{
    if(ProceduralSlabMesh)
    {
        ProceduralSlabMesh->SetRenderCustomDepth(true);
        ProceduralSlabMesh->SetCustomDepthStencilValue(2);
    }
}

void ASlabActor::UnhighlightDeselectedActor()
{
    if (ProceduralSlabMesh)
    {
        ProceduralSlabMesh->SetRenderCustomDepth(false);
    }
}

void ASlabActor::GenerateSlab()
{
    GenerateCube(0);
}
void ASlabActor::PlaceSlab(FVector NewLocation)
{
    // Snap the new location to the grid
    FVector SnappedLocation = Utility::SnapToGrid(NewLocation, FVector(20.0f));

    // Calculate the direction and distance between the original start and end points
    FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
    float Distance = FVector::Dist(StartLocation, EndLocation);

    // Set the new start point to the snapped location
    StartLocation = SnappedLocation;

    // Calculate the new end point based on the direction and distance
    EndLocation = StartLocation + Direction * Distance;

    FVector SpawnLocation = (StartLocation + EndLocation) * 0.5f; // Center point for spawning
    SpawnLocation.Z = StartLocation.Z;
    // Spawn the slabs at the new location
    ProceduralSlabMesh->SetRelativeLocation(StartLocation);
}

void ASlabActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    switch (WallState) {
    case EBuildingSubModeState::Placed:
        HandlePlacedState();
        break;
    case EBuildingSubModeState::Moving:
        HandleMovingState();
        break;
    case EBuildingSubModeState::Constructing:
        HandleConstructingState();
        break;
    }
}
void ASlabActor::HandlePlacedState()
{

}

void ASlabActor::HandleConstructingState()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {

        FHitResult HitResult;
        FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, this);

        FVector CursorWorldLocation;
        FVector CursorWorldDirection;
        PlayerController->DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

        if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
        {

            SetEndLocation(HitResult.Location);

            //CreateWallSegment();

        }

    }

}

void ASlabActor::HandleMovingState()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {

        FHitResult HitResult;
        FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, this);

        FVector CursorWorldLocation;
        FVector CursorWorldDirection;
        PlayerController->DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

        if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
        {
            FVector NewLocation = HitResult.Location;
            FVector SnappedLocation = Utility::SnapToGrid(NewLocation, FVector(10));
            SetActorLocation(SnappedLocation);
        }

    }
}