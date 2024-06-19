#include "WallActor.h"
#include "KismetProceduralMeshLibrary.h"


// Sets default values
AWallActor::AWallActor()
{
    PrimaryActorTick.bCanEverTick = true;
    ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
    RootComponent = ProceduralMesh;

    SnapPointStart = CreateDefaultSubobject<USceneComponent>(TEXT("SnapPointStart"));
    SnapPointStart->SetupAttachment(RootComponent);
    SnapPointStart->SetRelativeLocation(FVector(-WallLength / 2, 0, 0));

    SnapPointEnd = CreateDefaultSubobject<USceneComponent>(TEXT("SnapPointEnd"));
    SnapPointEnd->SetupAttachment(RootComponent);
    SnapPointEnd->SetRelativeLocation(FVector(WallLength / 2, 0, 0));

    IsDoorAdded = false;
    WallHeight = 100;
    WallWidth = 20;
    WallLength = 500;
    DoorWidth = 50.0f;
    DoorHeight = ((WallHeight * 2) / 3);
    DoorLocation = FVector::ZeroVector;

    
}

// Called when the game starts or when spawned
void AWallActor::BeginPlay()
{
    Super::BeginPlay();
    CreateWallMesh();
}

void AWallActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    CreateWallMesh();
}

// Called every frame
void AWallActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    CheckForSnapping();
}

void AWallActor::CheckForSnapping()
{
    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors, TSubclassOf<AWallActor>());

    for (AActor* Actor : OverlappingActors)
    {
        AWallActor* OtherWall = Cast<AWallActor>(Actor);
        if (OtherWall && OtherWall != this)
        {
            SnapToActor(OtherWall);
        }
    }
}

void AWallActor::SnapToActor(AWallActor* OtherWall)
{
    float SnapDistanceThreshold = 100.0f; // Define a suitable threshold

    FVector StartSnapPoint = SnapPointStart->GetComponentLocation();
    FVector EndSnapPoint = SnapPointEnd->GetComponentLocation();
    FVector OtherStartSnapPoint = OtherWall->SnapPointStart->GetComponentLocation();
    FVector OtherEndSnapPoint = OtherWall->SnapPointEnd->GetComponentLocation();

    if (FVector::Dist(EndSnapPoint, OtherStartSnapPoint) <= SnapDistanceThreshold)
    {
        FVector Offset = OtherStartSnapPoint - EndSnapPoint;
        SetActorLocation(GetActorLocation() + Offset);
    }
    else if (FVector::Dist(StartSnapPoint, OtherEndSnapPoint) <= SnapDistanceThreshold)
    {
        FVector Offset = OtherEndSnapPoint - StartSnapPoint;
        SetActorLocation(GetActorLocation() + Offset);
    }
}


void AWallActor::CreateWallMesh()
{

    float L = WallLength;  // Full width
    float W = WallWidth;  // Full depth
    float H = WallHeight;  // Full height
    float DoorLeft = DoorLocation.X - DoorWidth / 2;
    float DoorRight = DoorLocation.X + DoorWidth / 2;
    DoorWidth = FMath::Min(DoorWidth, WallHeight);
    DoorHeight = FMath::Min(DoorHeight, WallHeight - 10);

    // Ensure the door cut is within the wall bounds
    if (DoorLeft < -L / 2 || DoorRight > L / 2 || DoorHeight  > WallHeight )
    {
        UE_LOG(LogTemp, Warning, TEXT("Door location is out of bounds!"));
        return;
    }

    ProceduralMesh->ClearAllMeshSections();
    ResetArrays();

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

    if (IsDoorAdded)
    {
       
        Vertices.Add(FVector(-L / 2, -W / 2, H));   // 16
        Vertices.Add(FVector(DoorLeft, -W / 2, H));   // 17
        Vertices.Add(FVector(-L / 2, -W / 2, 0));   // 18
        Vertices.Add(FVector(DoorLeft, -W / 2, 0));   // 19

        Vertices.Add(FVector(L / 2, -W / 2, H));   // 20
        Vertices.Add(FVector(DoorRight, -W / 2, H));   // 21
        Vertices.Add(FVector(L / 2, -W / 2, 0));   // 22
        Vertices.Add(FVector(DoorRight, -W / 2, 0));//23

        Vertices.Add(FVector(-L / 2, W / 2, H));   // 24
        Vertices.Add(FVector(DoorLeft, W / 2, H));   // 25
        Vertices.Add(FVector(-L / 2, W / 2, 0));   // 26
        Vertices.Add(FVector(DoorLeft, W / 2, 0));   // 27

        Vertices.Add(FVector(L / 2, W / 2, H));   // 28
        Vertices.Add(FVector(DoorRight, W / 2, H));   // 29
        Vertices.Add(FVector(L / 2, W / 2, 0));   // 30
        Vertices.Add(FVector(DoorRight, W / 2, 0));//31

        Vertices.Add(FVector(DoorRight, -W / 2, H));   // 32
        Vertices.Add(FVector(DoorLeft, -W / 2, H));   // 33
        Vertices.Add(FVector(DoorRight, -W / 2, DoorHeight));   // 34
        Vertices.Add(FVector(DoorLeft, -W / 2, DoorHeight));//35

        Vertices.Add(FVector(DoorRight, W / 2, H));   // 36
        Vertices.Add(FVector(DoorLeft, W / 2, H));   // 37
        Vertices.Add(FVector(DoorRight, W / 2, DoorHeight));   // 38
        Vertices.Add(FVector(DoorLeft, W / 2, DoorHeight));//39

        Triangles = {
            0, 1, 3, 1, 2, 3,
            4, 5, 7, 5, 6, 7,
            8, 9, 11, 9, 10, 11,
            12, 13, 15, 13, 14, 15,

            // front portion
            24, 26, 39, 39, 26, 27,
            24, 39, 28, 39, 38, 28,
            28, 38, 30, 38, 31, 30,

            // back portion
            22, 34, 20, 23, 34, 22,
            16, 20, 34, 16, 34, 35,
            16, 35, 18, 18, 35, 19
        };
    }
    else
    {
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
            FVector2D(0, 0), FVector2D(0.5, 0), FVector2D(0.5, 1), FVector2D(0, 1),

            // Door top front face
            FVector2D(0.5, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0.5, 1)
        };

        // Door bottom back face
        UVs.Add(FVector2D(0, 0));
        UVs.Add(FVector2D(0.5, 0));
        UVs.Add(FVector2D(0, 1));
        UVs.Add(FVector2D(0.5, 1));

        // Door bottom front face
        UVs.Add(FVector2D(0.5, 0));
        UVs.Add(FVector2D(1, 0));
        UVs.Add(FVector2D(0.5, 1));
        UVs.Add(FVector2D(1, 1));
    }
    else
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
            FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1),

            // Door bottom back face
            FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1),

            // Door bottom front face
            FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1)
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

void AWallActor::SetDoorLocartion(float x)
{
    DoorLocation.X = x;
}


void AWallActor::SliceDoor()
{
    // Slice along the Z-axis (vertical plane)
    
}
