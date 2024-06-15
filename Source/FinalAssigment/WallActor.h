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

	void CreateWallMesh(float Length, float Breath, float Height);

	void ResetArrays();
	void AddQuad(int32 V0, int32 V1, int32 V2, int32 V3);
	void AddUVs();
	void AddNormals();

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* ProceduralMesh;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector2D> UVs;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> Vertices;
	
	UPROPERTY(VisibleAnywhere)
	TArray<int32> Triangles;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> Normals;

	UPROPERTY(EditAnywhere, Category = "Door")
	FVector DoorLocation;

	UPROPERTY(EditAnywhere, Category = "Door")
	bool IsDoorAdded;

	UPROPERTY(EditAnywhere, Category = "Door")
	float DoorWidth;

	UPROPERTY(EditAnywhere, Category = "Door")
	float DoorHeight;

	UPROPERTY(EditAnywhere, Category = "Door")
	UMaterialInterface* CapMaterial;

	void SliceDoor();
};
