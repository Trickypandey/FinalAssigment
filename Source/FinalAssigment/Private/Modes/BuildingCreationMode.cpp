// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/BuildingCreationMode.h"
#include "AssertManager.h"
#include "SlabActor.h"
#include "WallConstructionWidget.h"

UBuildingCreationMode::UBuildingCreationMode()
{
}

void UBuildingCreationMode::Setup()
{
    if (IsValid(WallCreationModeRef)) {
        WallCreationMode = NewObject<USubModeWallCreation>(this, WallCreationModeRef);
        WallCreationMode->InitParams(PlayerController);
        WallCreationMode->Setup();
    }
 
    if (IsValid(FloorCreationModeRef)) {
        FloorCreationMode = NewObject<USubModeFloorCreation>(this, FloorCreationModeRef);
        FloorCreationMode->InitParams(PlayerController);
        FloorCreationMode->Setup();
    }
    if (IsValid(CeilingModeRef)) {
        CeilingCreationMode = NewObject<USubModeCeilingCreation>(this, CeilingModeRef);
        CeilingCreationMode->InitParams(PlayerController);
        CeilingCreationMode->Setup();
    }
    if (IsValid(WidgetRef) && !IsValid(Widget)) {
        Widget = CreateWidget<UWallConstructionWidget>(PlayerController, WidgetRef);
    }
}

void UBuildingCreationMode::SetSubMode(UUBuildingCreationSubMode* NewSubMode) {
    if (CurrentBuildingCreationSubMode) {
        CurrentBuildingCreationSubMode->ExitSubMode(Cast<UWallConstructionWidget>(Widget));
    }

    CurrentBuildingCreationSubMode = NewSubMode;

    if (CurrentBuildingCreationSubMode) {
        CurrentBuildingCreationSubMode->EnterSubMode(Cast<UWallConstructionWidget>(Widget));
    }
}

void UBuildingCreationMode::SetupInputMapping()
{
    if (IsValid(WallCreationMode)) {
        WallCreationMode->SetupInputMapping();
    }
  
    if (IsValid(CeilingCreationMode)) {
        CeilingCreationMode->SetupInputMapping();
    }
    if (IsValid(FloorCreationMode)) {
        FloorCreationMode->SetupInputMapping();
    }
}

void UBuildingCreationMode::EnterMode()
{
	Super::ShowWidget();
}

void UBuildingCreationMode::ExitMode()
{
	Super::HideWidget();
}

ACubeActor*& UBuildingCreationMode::GetSelectedActor()
{
    return CurrentBuildingCreationSubMode->GetSelectedActor();
}



void UBuildingCreationMode::SpawnSelectedActor(EBuildingCreationType Type)
{

    switch (Type) {
	    case EBuildingCreationType::Wall:
				SetSubMode(WallCreationMode);
        
		    break;
	    case EBuildingCreationType::Floor:
				SetSubMode(FloorCreationMode);
		    break;
	    case EBuildingCreationType::Ceiling:
				SetSubMode(CeilingCreationMode);
		    break;
    }
}

void UBuildingCreationMode::ApplyMaterialWallProceduralMesh(const FMaterialData& MeshData)
{
    UMaterialInterface* BaseMaterial = MeshData.Type;
    if (!BaseMaterial)
    {
        UE_LOG(LogTemp, Error, TEXT("BaseMaterial is nullptr in AArchVizPlayerController"));
    }

    if (auto DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this))
    {
	    if (CurrentBuildingCreationSubMode)
	    {
    		CurrentBuildingCreationSubMode->SetMaterial(DynamicMaterial);
	    }
    }
}