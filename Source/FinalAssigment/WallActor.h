// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "WallActor.generated.h"

UCLASS()
class FINALASSIGMENT_API AWallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CheckForSnapping();
	void SnapToActor(AWallActor* OtherWall);

	void CreateWallMesh();

	void ResetArrays();
	void AddQuad(int32 V0, int32 V1, int32 V2, int32 V3);
	void AddUVs();
	void AddNormals();
	void SetDoorLocation(float x);
	void SetIsDoorAdded(bool);

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* ProceduralMesh;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SnapPointStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SnapPointEnd;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector2D> UVs;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> Vertices;
	
	UPROPERTY(VisibleAnywhere)
	TArray<int32> Triangles;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> Normals;

	UPROPERTY(EditAnywhere, Category = "Property")
	int32 WallLength;

	UPROPERTY(EditAnywhere, Category = "Property")
	int32 WallWidth;

	UPROPERTY(EditAnywhere, Category = "Property")
	int32 WallHeight;

	UPROPERTY(EditAnywhere, Category = "Property")
	FVector DoorLocation;

	UPROPERTY(EditAnywhere, Category = "Property")
	bool IsDoorAdded;

	UPROPERTY(EditAnywhere, Category = "Property")
	float DoorWidth;

	UPROPERTY(EditAnywhere, Category = "Property")
	float DoorHeight;

	UPROPERTY(EditAnywhere, Category = "Property")
	UMaterialInterface* CapMaterial;

	void SliceDoor();
};
