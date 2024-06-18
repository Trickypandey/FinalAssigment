#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "ProceduralMeshComponent.h"
#include "RoadActor.generated.h"

UCLASS()
class FINALASSIGMENT_API ARoadActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ARoadActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USplineComponent* SplineComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UProceduralMeshComponent* ProceduralMeshComponent;

    // Function to generate the road mesh
    void GenerateRoadMesh();

    // Function to add a spline point
    UFUNCTION(BlueprintCallable, Category = "Spline")
    void AddSplinePoint(FVector Location);

    // Function to set the material for the road
    UFUNCTION(BlueprintCallable, Category = "Materials")
    void SetMaterialForSection(int32 SectionIndex, UMaterialInterface* Material);

    // Road dimensions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road Dimensions")
    float RoadWidth = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road Dimensions")
    float RoadThickness = 10.0f;

    // Material for the road
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    UMaterialInterface* RoadMaterial;
};
