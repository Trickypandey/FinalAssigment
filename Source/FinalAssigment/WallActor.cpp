#include "WallActor.h"

// Sets default values
AWallActor::AWallActor()
{
    PrimaryActorTick.bCanEverTick = true;
    ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
    RootComponent = ProceduralMesh;
}

// Called when the game starts or when spawned
void AWallActor::BeginPlay()
{
    Super::BeginPlay();
    CreateWallMesh(500, 10, 100);
}

void AWallActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    CreateWallMesh(300, 10, 100);
}

// Called every frame
void AWallActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AWallActor::CreateWallMesh(float Length, float Breath, float Height)
{
    ProceduralMesh->ClearAllMeshSections();
    ResetArrays();

    float L = Length;  // Half width
    float W = Breath;  // Half depth
    float H = Height * 2;     // Full height


    Vertices.Add(FVector(-L, -W, 0));  // 0
    Vertices.Add(FVector(-L, W, 0));   // 1
    Vertices.Add(FVector(-L, W, H));   // 2
    Vertices.Add(FVector(-L, -W, H));  // 3


    Vertices.Add(FVector(-L, -W, 0));  // 4
    Vertices.Add(FVector(L, -W, 0));   // 5
    Vertices.Add(FVector(L, W, 0));    // 6
    Vertices.Add(FVector(-L, W, 0));   // 7


    Vertices.Add(FVector(L, -W, 0));   // 8
    Vertices.Add(FVector(L, -W, H));   // 9
    Vertices.Add(FVector(L, W, H));    // 10
    Vertices.Add(FVector(L, W, 0));    // 11


    Vertices.Add(FVector(L, -W, H));   // 12
    Vertices.Add(FVector(-L, -W, H));  // 13
    Vertices.Add(FVector(-L, W, H));   // 14
    Vertices.Add(FVector(L, W, H));    // 15


    Vertices.Add(FVector(-L, -W, H));  // 16
    Vertices.Add(FVector(L, -W, H));   // 17
    Vertices.Add(FVector(L, -W, 0));   // 18
    Vertices.Add(FVector(-L, -W, 0));  // 19


    Vertices.Add(FVector(-L, W, H));   // 20
    Vertices.Add(FVector(-L, W, 0));   // 21
    Vertices.Add(FVector(L, W, 0));    // 22
    Vertices.Add(FVector(L, W, H));    // 23


    Triangles = {
    0, 1, 3, 1, 2, 3,
    4, 5, 7, 5, 6, 7,
    8, 9, 11, 9, 10, 11,
    12, 13, 15, 13, 14, 15,
    16, 17, 19, 17, 18, 19,
    20, 21, 23, 21, 22, 23
    };




    if (IsDoorAdded)
    {
        ResetArrays();

        // left face
        Vertices.Add(FVector(-L, -W, 0));  // 0
        Vertices.Add(FVector(-L, W, 0));   // 1
        Vertices.Add(FVector(-L, W, H));   // 2
        Vertices.Add(FVector(-L, -W, H));  // 3

        // bottom face
        Vertices.Add(FVector(-L, -W, 0));  // 4
        Vertices.Add(FVector(L, -W, 0));   // 5
        Vertices.Add(FVector(L, W, 0));    // 6
        Vertices.Add(FVector(-L, W, 0));   // 7

        // right face
        Vertices.Add(FVector(L, -W, 0));   // 8
        Vertices.Add(FVector(L, -W, H));   // 9
        Vertices.Add(FVector(L, W, H));    // 10
        Vertices.Add(FVector(L, W, 0));    // 11

        // top 
        Vertices.Add(FVector(L, -W, H));   // 12
        Vertices.Add(FVector(-L, -W, H));  // 13
        Vertices.Add(FVector(-L, W, H));   // 14
        Vertices.Add(FVector(L, W, H));    // 15

        // back left
        Vertices.Add(FVector(-L, -W, H));   // 16
        Vertices.Add(FVector(-L / 4, -W, H));   // 17
        Vertices.Add(FVector(-L, -W, 0));   // 18
        Vertices.Add(FVector(-L / 4, -W, 0));   // 19


        // back right
        Vertices.Add(FVector(L, -W, H));   // 20
        Vertices.Add(FVector(L / 4, -W, H));   // 21
        Vertices.Add(FVector(L, -W, 0));   // 22
        Vertices.Add(FVector(L / 4, -W, 0));//23

        // front left portion
        Vertices.Add(FVector(-L, W, H));   // 24
        Vertices.Add(FVector(-L / 4, W, H));   // 25
        Vertices.Add(FVector(-L, W, 0));   // 26
        Vertices.Add(FVector(-L / 4, W, 0));   // 27

        // front right portion
        Vertices.Add(FVector(L, W, H));   // 28
        Vertices.Add(FVector(L / 4, W, H));   // 29
        Vertices.Add(FVector(L, W, 0));   // 30
        Vertices.Add(FVector(L / 4, W, 0));//31


        // door Top Back
        Vertices.Add(FVector(L / 4, -W, H));   // 32
        Vertices.Add(FVector(-L / 4, -W, H));   // 33
        Vertices.Add(FVector(L / 4, -W, (H * 2) / 3));   // 34
        Vertices.Add(FVector(-L / 4, -W, (H * 2) / 3));//35




        // door Top front
        Vertices.Add(FVector(L / 4, W, H));   // 36
        Vertices.Add(FVector(-L / 4, W, H));   // 37
        Vertices.Add(FVector(L / 4, W, (H * 2) / 3));   // 38
        Vertices.Add(FVector(-L / 4, W, (H * 2) / 3));//39
        // door Top Bottom






        Triangles = {
        0, 1, 3, 1, 2, 3,
        4, 5, 7, 5, 6, 7,
        8, 9, 11, 9, 10, 11,
        12, 13, 15, 13, 14, 15,
        19, 16, 17, 19, 18, 16,
        21, 20, 22, 21, 22, 23,
        25, 24, 26, 25, 26, 27,
        29,30,28,29,31,30,
            32,34,33,33,34,35,
            36,37,38,37,39,38

        };

    }

    AddNormals();
    AddUVs();

    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
}

void AWallActor::ResetArrays()
{
    UVs.Empty();
    Vertices.Empty();
    Triangles.Empty();
    Normals.Empty();
}

void AWallActor::AddQuad(int32 V0, int32 V1, int32 V2, int32 V3)
{
    Triangles.Add(V0);
    Triangles.Add(V1);
    Triangles.Add(V2);
    Triangles.Add(V2);
    Triangles.Add(V3);
    Triangles.Add(V0);
}

void AWallActor::AddUVs()
{
    UVs.Empty();

    if (IsDoorAdded)
    {
        // Define UVs for faces with the door added
        UVs = {
            // Left face
            FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),

            // Bottom face
            FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),

            // Right face
            FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),

            // Top face
            FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),

            // Back left face
            FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),

            // Back right face
            FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),

            // Front left face
            FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),

            // Front right face
            FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),

            // Door top back face
            FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),

            // Door top front face
            FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1)
        };
    }
    else
    {
        // Define UVs for faces without the door added
        UVs = {
            FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),
            FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),
            FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),
            FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),
            FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),
            FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1)
        };
    }
}


void AWallActor::AddNormals()
{
    Normals.Empty();
    if (IsDoorAdded)
    {
        Normals = {
            // Left face
            FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0),

            // Bottom face
            FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1),

            // Right face
            FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0),

            // Top face
            FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1),

            // Back left face
            FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0),

            // Back right face
            FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0),

            // Front left face
            FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0),

            // Front right face
            FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0),

            // Door top back face
            FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1),

            // Door top front face
            FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1)
        };

       
    }
    else
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
}
