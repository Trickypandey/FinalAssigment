// Fill out your copyright notice in the Description page of Project Settings.


#include "SlabActor.h"

ASlabActor::ASlabActor()
{
    PrimaryActorTick.bCanEverTick = true;
    Length = 500.0f;
    Width = 500.0f;
    Height = 10.0f; // Typical slab thickness
}

void ASlabActor::BeginPlay()
{
    Super::BeginPlay();
    CreateSlabMesh();
}

void ASlabActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    CreateSlabMesh();
}

void ASlabActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASlabActor::CreateSlabMesh()
{
    CreateCubeMesh(); 
}
