// Fill out your copyright notice in the Description page of Project Settings.


#include "InteriorDesignActor.h"

// Sets default values
AInteriorDesignActor::AInteriorDesignActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteriorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Interior Mesh Component"));
	RootComponent = InteriorMeshComponent;

}

UStaticMesh* AInteriorDesignActor::GetStaticMesh()
{
	return InteriorMeshComponent->GetStaticMesh();
}

void AInteriorDesignActor::SetStaticMesh(UStaticMesh* StaticMesh)
{
	if (InteriorMeshComponent && StaticMesh)
		InteriorMeshComponent->SetStaticMesh(StaticMesh);
}

// Called when the game starts or when spawned
void AInteriorDesignActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteriorDesignActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

