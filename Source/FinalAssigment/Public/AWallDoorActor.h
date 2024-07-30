// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "AWallDoorActor.generated.h"

UCLASS()
class FINALASSIGMENT_API AAWallDoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AAWallDoorActor();
	void SetDoorLocation(float X);
	FVector GetDoorLocation();
	void AttachDoor(UStaticMesh*& DoorStaticMesh);

protected:
	
	virtual void BeginPlay() override;
private:
	UPROPERTY()
	AActor* ParentActor;

public:
	void SetParentActor(AActor* ParentActor);
	AActor* GetParentActor();
	
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void AddNormals(TArray<FVector>& Array);
	void AddUVs(TArray<FVector2D>& Array);
	void CreateMesh();
	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* DoorMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess))
	UStaticMesh* DoorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
	FVector DoorLocation{ 0 };

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
	float DoorWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
	float DoorHeight;
	UProceduralMeshComponent* ProceduralMesh;
};
