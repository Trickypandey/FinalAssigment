#include "RoadActor.h"

// Sets default values
ARoadActor::ARoadActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create and initialize the spline component
    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    RootComponent = SplineComponent;

    // Create and initialize the procedural mesh component
    ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
    ProceduralMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARoadActor::BeginPlay()
{
    Super::BeginPlay();
    GenerateRoadMesh();
}

void ARoadActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    GenerateRoadMesh();
}

// Called every frame
void ARoadActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARoadActor::GenerateRoadMesh()
{
    ProceduralMeshComponent->ClearAllMeshSections();
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;

    const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
    for (int32 i = 0; i < NumPoints - 1; ++i)
    {
        FVector StartLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
        FVector EndLocation = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
        FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
        FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

        float SegmentLength = (EndLocation - StartLocation).Size();
        FVector RightVector = FVector::CrossProduct(StartTangent, FVector::UpVector).GetSafeNormal();
        FVector UpVector = FVector::UpVector;

        // Create vertices for this segment
        Vertices.Add(StartLocation - RightVector * RoadWidth / 2);
        Vertices.Add(StartLocation + RightVector * RoadWidth / 2);
        Vertices.Add(EndLocation - RightVector * RoadWidth / 2);
        Vertices.Add(EndLocation + RightVector * RoadWidth / 2);

        int32 VertexIndex = Vertices.Num();

        // Create triangles
        Triangles.Add(VertexIndex - 4);
        Triangles.Add(VertexIndex - 3);
        Triangles.Add(VertexIndex - 2);

        Triangles.Add(VertexIndex - 3);
        Triangles.Add(VertexIndex - 1);
        Triangles.Add(VertexIndex - 2);

        // Create normals
        Normals.Add(UpVector);
        Normals.Add(UpVector);
        Normals.Add(UpVector);
        Normals.Add(UpVector);

        // Create UVs
        UVs.Add(FVector2D(0.0f, 0.0f));
        UVs.Add(FVector2D(1.0f, 0.0f));
        UVs.Add(FVector2D(0.0f, 1.0f));
        UVs.Add(FVector2D(1.0f, 1.0f));
    }

    ProceduralMeshComponent->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
    if (RoadMaterial)
    {
        ProceduralMeshComponent->SetMaterial(0, RoadMaterial);
    }
}

void ARoadActor::AddSplinePoint(FVector Location)
{
    SplineComponent->AddSplinePoint(Location, ESplineCoordinateSpace::World);
}

void ARoadActor::SetMaterialForSection(int32 SectionIndex, UMaterialInterface* Material)
{
    if (Material)
    {
        ProceduralMeshComponent->SetMaterial(SectionIndex, Material);
    }
}
