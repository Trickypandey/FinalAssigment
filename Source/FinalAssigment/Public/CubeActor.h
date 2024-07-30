// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "CubeActor.generated.h"

UCLASS()
class FINALASSIGMENT_API ACubeActor : public AActor
{
	GENERATED_BODY()
	
public:
    ACubeActor();

    void SetLength(float length);
    float GetLength();
	float GetWidth();
    UProceduralMeshComponent* GetProceduralMeshComponent();

	void SetWidth(float width);
    virtual void CreateMesh();
	void SetDimension(int32 NewLength, int32 NewWidth, int32 NewHeight);

    void SetParentActor(AActor* ParentActor);
    AActor* GetParentActor();
    
private:
    UPROPERTY()
    AActor* ParentActor;

protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;


public:
    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UProceduralMeshComponent* ProceduralMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Property")
    float Length;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Property")
    float Width;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Property")
    float Height;

    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Property")
    TArray<FVector2D> UVs;

	virtual void ResetArrays();
	virtual void AddQuad(int32 V0, int32 V1, int32 V2, int32 V3);
	virtual void AddUVs();
	virtual void AddNormals();
    

};
