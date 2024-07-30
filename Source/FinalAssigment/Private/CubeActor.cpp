#include "CubeActor.h"

ACubeActor::ACubeActor()
{
    PrimaryActorTick.bCanEverTick = true;

    ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
    RootComponent = ProceduralMesh;

    Length = 100;
    Width = 100;
    Height = 100;
}

void ACubeActor::SetLength(float length)
{
    this->Length = length;
    CreateMesh();
}

float ACubeActor::GetLength()
{
    return Length;
}

float ACubeActor::GetWidth()
{
    return Width;
}

UProceduralMeshComponent* ACubeActor::GetProceduralMeshComponent()
{
    return ProceduralMesh;
}

void ACubeActor::SetWidth(float width)
{
    this->Width = width;
    CreateMesh();

}

void ACubeActor::BeginPlay()
{
    Super::BeginPlay();
    CreateMesh();
}

void ACubeActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    CreateMesh();
}

void ACubeActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACubeActor::CreateMesh()
{
    ProceduralMesh->ClearAllMeshSections();
    ResetArrays();

    float L = Length;
    float W = Width;
    float H = Height;

    Vertices.Add(FVector(-L / 2, -W / 2, 0));  // 0
    Vertices.Add(FVector(-L / 2, W / 2, 0));   // 1
    Vertices.Add(FVector(-L / 2, W / 2, H));   // 2
    Vertices.Add(FVector(-L / 2, -W / 2, H));  // 3

    Vertices.Add(FVector(-L / 2, -W / 2, 0));  // 4
    Vertices.Add(FVector(L / 2, -W / 2, 0));   // 5
    Vertices.Add(FVector(L / 2, W / 2, 0));    // 6
    Vertices.Add(FVector(-L / 2, W / 2, 0));   // 7

    Vertices.Add(FVector(L / 2, -W / 2, 0));   // 8
    Vertices.Add(FVector(L / 2, -W / 2, H));   // 9
    Vertices.Add(FVector(L / 2, W / 2, H));    // 10
    Vertices.Add(FVector(L / 2, W / 2, 0));    // 11

    Vertices.Add(FVector(L / 2, -W / 2, H));   // 12
    Vertices.Add(FVector(-L / 2, -W / 2, H));  // 13
    Vertices.Add(FVector(-L / 2, W / 2, H));   // 14
    Vertices.Add(FVector(L / 2, W / 2, H));    // 15

    Vertices.Add(FVector(-L / 2, -W / 2, H));  // 16
    Vertices.Add(FVector(L / 2, -W / 2, H));   // 17
    Vertices.Add(FVector(L / 2, -W / 2, 0));   // 18
    Vertices.Add(FVector(-L / 2, -W / 2, 0));  // 19

    Vertices.Add(FVector(-L / 2, W / 2, H));   // 20
    Vertices.Add(FVector(-L / 2, W / 2, 0));   // 21
    Vertices.Add(FVector(L / 2, W / 2, 0));    // 22
    Vertices.Add(FVector(L / 2, W / 2, H));    // 23

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
void ACubeActor::SetDimension(int32 NewLength, int32 NewWidth, int32 NewHeight)
{
    Length = NewLength;
    Width = NewWidth;
    Height = NewHeight;

    // Update the mesh or any other necessary properties
    CreateMesh();
}

void ACubeActor::SetParentActor(AActor* parentActor)
{
    ParentActor = parentActor;
}

AActor* ACubeActor::GetParentActor()
{
    return ParentActor;
}

void ACubeActor::ResetArrays()
{
    Vertices.Empty();
    Triangles.Empty();
    Normals.Empty();
    UVs.Empty();
}

void ACubeActor::AddQuad(int32 V0, int32 V1, int32 V2, int32 V3)
{
    Triangles.Add(V0);
    Triangles.Add(V1);
    Triangles.Add(V2);
    Triangles.Add(V2);
    Triangles.Add(V3);
    Triangles.Add(V0);
}

void ACubeActor::AddUVs()
{
    UVs = {
               FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),
               FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),
               FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),
               FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),
               FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),
               FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1)
    };
}

void ACubeActor::AddNormals()
{
    Normals = {
             FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0),
             FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1),
             FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0),
             FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1),
             FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0),
             FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0)
    };
}
