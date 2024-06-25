// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteriorDesignActor.generated.h"

UCLASS()
class FINALASSIGMENT_API AInteriorDesignActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteriorDesignActor();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior")
	UStaticMesh* InteriorStaticMesh;
	UStaticMesh* GetStaticMesh();
	void SetStaticMesh(UStaticMesh* StaticMesh);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interior")
	UStaticMeshComponent* InteriorMeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
