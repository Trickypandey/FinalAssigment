#include "WallActor.h"

AWallActor::AWallActor()
{
    PrimaryActorTick.bCanEverTick = true;
    ConstructionType = EBuildingCreationType::Wall;
    WallState = EBuildingSubModeState::Moving;
    Length = 400;
    Width = 20;
    Height = 400;
    IsDoorAdded = false;
    DoorWidth = 90.0f;
    DoorHeight = 200.0f; 
    DoorLocation = FVector(100.0f, 0.0f, 0.0f);
    DoorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMeshComponent->SetupAttachment(RootComponent);

    DoorMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Props/SM_Door.SM_Door"));
}

void AWallActor::BeginPlay()
{
    Super::BeginPlay();
    CreateMesh();
    if (DoorMesh)
    {
        AttachDoor(DoorMesh);
    }
}

void AWallActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    CreateMesh();
    if (DoorMesh)
    {
        AttachDoor(DoorMesh);
    }
}

void AWallActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    switch (WallState) {
	    case EBuildingSubModeState::Placed:
	        HandlePlacedState();
	        break;
	    case EBuildingSubModeState::Moving:
	        HandleMovingState();
	        break;
    }

   
}

void AWallActor::SetDoorLocation(float X)
{
    DoorLocation.X = X;
    CreateMesh();
}

void AWallActor::SetIsDoorAdded(bool Flag)
{
    IsDoorAdded = Flag;
    CreateMesh();
}

void AWallActor::SetDimension(int32 _Length, int32 _Width)
{
    this->Length = _Length;
    this->Width = _Width;
    CreateMesh();
}

void AWallActor::AttachDoor(UStaticMesh*& DoorStaticMesh)
{
    if (DoorStaticMesh)
    {
        DoorMeshComponent->SetStaticMesh(DoorStaticMesh);
    }
}

void AWallActor::HandleMovingState()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        
        FHitResult HitResult;
        FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true,this);

        FVector CursorWorldLocation;
        FVector CursorWorldDirection;
        PlayerController->DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

        if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
        {
            FVector NewLocation =Utility::SnapToGrid(HitResult.Location,FVector(20));
            SetActorLocation(NewLocation);
        }
        
    }
}


void AWallActor::HandlePlacedState()
{
}

void AWallActor::CreateMesh()
{
    float L = Length;  // Full width
    float W = Width;  // Full depth
    float H = Height;  // Full height
    float DoorLeft = DoorLocation.X - DoorWidth / 2;
    float DoorRight = DoorLocation.X + DoorWidth / 2;
    DoorWidth = FMath::Min(DoorWidth, Height);
    DoorHeight = FMath::Min(DoorHeight, Height - 10);

    // Ensure the door cut is within the wall bounds
    if (DoorLeft < -L / 2 || DoorRight > L / 2 || DoorHeight > Height)
    {
        UE_LOG(LogTemp, Warning, TEXT("Door location is out of bounds!"));
        return;
    }

    ProceduralMesh->ClearAllMeshSections();
    ResetArrays();
    DoorMeshComponent->SetRelativeLocation(FVector(DoorRight,DoorLocation.Y,DoorLocation.Z));
    DoorMeshComponent->SetRelativeRotation(FRotator(0, -90, 0));

    if (IsDoorAdded)
    {
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


        // door left
        Vertices.Add(FVector(-L / 2, W / 2, DoorHeight));   // 16
        Vertices.Add(FVector(DoorLeft, W / 2, DoorHeight));   // 17
        Vertices.Add(FVector(DoorLeft, W / 2, 0));   // 18
        Vertices.Add(FVector(-L/2 , W / 2, 0));   // 19


        // door top
        Vertices.Add(FVector(-L / 2, W / 2, H));   // 20
        Vertices.Add(FVector(L / 2, W / 2, H));   // 21
        Vertices.Add(FVector(L / 2, W / 2, DoorHeight));   // 22
        Vertices.Add(FVector(-L / 2, W / 2, DoorHeight));//23

        // door right 
        Vertices.Add(FVector(DoorRight, W / 2, DoorHeight));   // 24
        Vertices.Add(FVector(L/2, W / 2, DoorHeight));   // 25
        Vertices.Add(FVector(L / 2, W / 2, 0));   // 26
        Vertices.Add(FVector(DoorRight, W / 2, 0));   // 27

        // BACK LEFT 
        Vertices.Add(FVector(-L / 2, -W / 2, DoorHeight));   // 28
        Vertices.Add(FVector(DoorLeft, -W / 2, DoorHeight));   // 29
        Vertices.Add(FVector(DoorLeft, -W / 2, 0));   // 30
        Vertices.Add(FVector(-L/2, -W / 2, 0));//31


        // BACK RIGHT
        Vertices.Add(FVector(DoorRight, -W / 2, DoorHeight));   // 32
        Vertices.Add(FVector(L/2, -W / 2, DoorHeight));   // 33
        Vertices.Add(FVector(L/2, -W / 2, 0));   // 34
        Vertices.Add(FVector(DoorRight, -W / 2, 0));//35

        // back top

        Vertices.Add(FVector(-L/2, -W / 2, H));   // 36
        Vertices.Add(FVector(L/2, -W / 2, H));   // 37
        Vertices.Add(FVector(L/2,- W / 2, DoorHeight));   // 38
        Vertices.Add(FVector(-L/2, -W / 2, DoorHeight));//39

        Triangles = {
            0, 1, 3, 1, 2, 3,
            4, 5, 7, 5, 6, 7,
            8, 9, 11, 9, 10, 11,
            12, 13, 15, 13, 14, 15,
// door left
         16,19,18,16,18,17,

            // door top
            20,23,22,20,22,21,

            // door ight
            24,27,26,24,26,25,

            // BACK LEFT

            30,31,28,29,30,28,

            // back right
            34,35,32,33,34,32,

            // back top

            38,39,36,37,38,36

           /* 24, 26, 39, 39, 26, 27,
            24, 39, 28, 39, 38, 28,
            28, 38, 30, 38, 31, 30,

            22, 34, 20, 23, 34, 22,
            16, 20, 34, 16, 34, 35,
            16, 35, 18, 18, 35, 19*/
        };

    }
    else
    {
        
        Super::CreateMesh();
    }

    AddNormals();
    AddUVs();

    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
    
}


void AWallActor::ResetArrays()
{
    Vertices.Empty();
    Triangles.Empty();
    Normals.Empty();
    UVs.Empty();
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
    if (IsDoorAdded)
    {
        // Front face
        UVs.Add(FVector2D(0.0f, 0.0f)); // 0
        UVs.Add(FVector2D(1.0f, 0.0f)); // 1
        UVs.Add(FVector2D(1.0f, 1.0f)); // 2
        UVs.Add(FVector2D(0.0f, 1.0f)); // 3

        // Bottom face
        UVs.Add(FVector2D(0.0f, 0.0f)); // 4
        UVs.Add(FVector2D(1.0f, 0.0f)); // 5
        UVs.Add(FVector2D(1.0f, 1.0f)); // 6
        UVs.Add(FVector2D(0.0f, 1.0f)); // 7

        // Right face
        UVs.Add(FVector2D(0.0f, 0.0f)); // 8
        UVs.Add(FVector2D(1.0f, 0.0f)); // 9
        UVs.Add(FVector2D(1.0f, 1.0f)); // 10
        UVs.Add(FVector2D(0.0f, 1.0f)); // 11

        // Top face
        UVs.Add(FVector2D(0.0f, 0.0f)); // 12
        UVs.Add(FVector2D(1.0f, 0.0f)); // 13
        UVs.Add(FVector2D(1.0f, 1.0f)); // 14
        UVs.Add(FVector2D(0.0f, 1.0f)); // 15

        // Door left
        UVs.Add(FVector2D(0.0f, 0.0f)); // 16
        UVs.Add(FVector2D(1.0f, 0.0f)); // 17
        UVs.Add(FVector2D(1.0f, 1.0f)); // 18
        UVs.Add(FVector2D(0.0f, 1.0f)); // 19

        // Door top
        UVs.Add(FVector2D(0.0f, 0.0f)); // 20
        UVs.Add(FVector2D(1.0f, 0.0f)); // 21
        UVs.Add(FVector2D(1.0f, 1.0f)); // 22
        UVs.Add(FVector2D(0.0f, 1.0f)); // 23

        // Door right
        UVs.Add(FVector2D(0.0f, 0.0f)); // 24
        UVs.Add(FVector2D(1.0f, 0.0f)); // 25
        UVs.Add(FVector2D(1.0f, 1.0f)); // 26
        UVs.Add(FVector2D(0.0f, 1.0f)); // 27

        // Back right
        UVs.Add(FVector2D(1.0f, 1.0f)); // 25
        UVs.Add(FVector2D(0.0f, 1.0f)); // 24
        UVs.Add(FVector2D(1.0f, 0.0f)); // 26
        UVs.Add(FVector2D(0.0f, 0.0f)); // 27

        // Back left
        UVs.Add(FVector2D(0.0f, 1.0f)); // 28
        UVs.Add(FVector2D(1.0f, 1.0f)); // 29
        UVs.Add(FVector2D(1.0f, 0.0f)); // 30
        UVs.Add(FVector2D(0.0f, 0.0f)); // 31

        // Back top
        UVs.Add(FVector2D(0.0f, 1.0f)); // 32
        UVs.Add(FVector2D(1.0f, 1.0f)); // 33
        UVs.Add(FVector2D(1.0f, 0.0f)); // 34
        UVs.Add(FVector2D(0.0f, 0.0f)); // 35
    }
    else
    {
        Super::AddUVs();
    }
}



void AWallActor::AddNormals()
{
    Super::AddNormals();
}
