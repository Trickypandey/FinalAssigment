// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CubeActor.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "WallActor.generated.h"

UCLASS()
class FINALASSIGMENT_API AWallActor : public ACubeActor
{
	GENERATED_BODY()
	
public:
    AWallActor();

protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Wall")
    void SetDoorLocation(float X);

    UFUNCTION(BlueprintCallable, Category = "Wall")
    void SetIsDoorAdded(bool Flag);

    UFUNCTION(BlueprintCallable, Category = "Wall")
    void SetDimension(int32 _Length, int32 _Width);

    void CreateWallMesh();
private:
	virtual void ResetArrays() override;
	virtual void AddQuad(int32 V0, int32 V1, int32 V2, int32 V3) override;
	virtual void AddUVs() override;
	virtual void AddNormals() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
    FVector DoorLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
    bool IsDoorAdded;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
    float DoorWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall", meta = (AllowPrivateAccess = "true"))
    float DoorHeight;
};
