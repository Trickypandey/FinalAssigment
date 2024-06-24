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
    const int32 SubdivisionsPerSegment = 10; // Number of subdivisions per spline segment
    RoadWidth = 100.0f; // Adjust this to your desired road width
    const float RoadHeight = 15.0f; // Adjust this to your desired road height

    for (int32 i = 0; i < NumPoints - 1; ++i)
    {
        FVector StartLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
        FVector EndLocation = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

        FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
        FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

        // Calculate the angle between the tangents of the current and next spline segments
        float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(StartTangent.GetSafeNormal(), EndTangent.GetSafeNormal())));

        int32 NumSubdivisions = FMath::Max(1, FMath::CeilToInt(Angle / 15.0f) * SubdivisionsPerSegment);

        for (int32 j = 0; j <= NumSubdivisions; ++j)
        {
            float Alpha = static_cast<float>(j) / NumSubdivisions;
            FVector CurrentLocation = FMath::CubicInterp(StartLocation, StartTangent, EndLocation, EndTangent, Alpha);
            FVector CurrentTangent = FMath::CubicInterpDerivative(StartLocation, StartTangent, EndLocation, EndTangent, Alpha).GetSafeNormal();

            FVector RightVector = FVector::CrossProduct(CurrentTangent, FVector::UpVector).GetSafeNormal();
            FVector UpVector = FVector::UpVector;

            // Create vertices for this segment
            FVector BottomLeft = CurrentLocation - RightVector * RoadWidth / 2 - UpVector * RoadHeight / 2;
            FVector BottomRight = CurrentLocation + RightVector * RoadWidth / 2 - UpVector * RoadHeight / 2;
            FVector TopLeft = CurrentLocation - RightVector * RoadWidth / 2 + UpVector * RoadHeight / 2;
            FVector TopRight = CurrentLocation + RightVector * RoadWidth / 2 + UpVector * RoadHeight / 2;

            Vertices.Add(BottomLeft);
            Vertices.Add(BottomRight);
            Vertices.Add(TopLeft);
            Vertices.Add(TopRight);

            int32 VertexIndex = Vertices.Num();

            if (j > 0)
            {
                // Create triangles for the top surface
                Triangles.Add(VertexIndex - 6);
                Triangles.Add(VertexIndex - 8);
                Triangles.Add(VertexIndex - 4);

                Triangles.Add(VertexIndex - 2);
                Triangles.Add(VertexIndex - 6);
                Triangles.Add(VertexIndex - 4);

                // Create triangles for the bottom surface
                Triangles.Add(VertexIndex - 7);
                Triangles.Add(VertexIndex - 5);
                Triangles.Add(VertexIndex - 3);

                Triangles.Add(VertexIndex - 5);
                Triangles.Add(VertexIndex - 1);
                Triangles.Add(VertexIndex - 3);

                // Create triangles for the left side surface
                Triangles.Add(VertexIndex - 8);
                Triangles.Add(VertexIndex - 7);
                Triangles.Add(VertexIndex - 5);

                Triangles.Add(VertexIndex - 8);
                Triangles.Add(VertexIndex - 5);
                Triangles.Add(VertexIndex - 4);

                // Create triangles for the right side surface
                Triangles.Add(VertexIndex - 1);
                Triangles.Add(VertexIndex - 6);
                Triangles.Add(VertexIndex - 2);

                Triangles.Add(VertexIndex - 5);
                Triangles.Add(VertexIndex - 6);
                Triangles.Add(VertexIndex - 1);

                // Create normals
                Normals.Add(UpVector);
                Normals.Add(UpVector);
                Normals.Add(UpVector);
                Normals.Add(UpVector);
            }

            // Create UVs
            float UVX = Alpha;
            UVs.Add(FVector2D(UVX, 0.0f)); // BottomLeft
            UVs.Add(FVector2D(UVX, 1.0f)); // BottomRight
            UVs.Add(FVector2D(UVX, 0.0f)); // TopLeft
            UVs.Add(FVector2D(UVX, 1.0f)); // TopRight
        }
    }

    ProceduralMeshComponent->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
    if (RoadMaterial)
    {
        ProceduralMeshComponent->SetMaterial(0, RoadMaterial);
    }
}







void ARoadActor::AddSplinePoint(FVector Location)
{
    
    Location.Z = 0.0f;

    
    SplineComponent->AddSplinePoint(Location, ESplineCoordinateSpace::World);
    if(SplineComponent->GetNumberOfSplinePoints() > 1)
    {
        GenerateRoadMesh();
    }
}

void ARoadActor::SetMaterialForSection(int32 SectionIndex, UMaterialInterface* Material)
{
    if (Material)
    {
        ProceduralMeshComponent->SetMaterial(SectionIndex, Material);
    }
}
