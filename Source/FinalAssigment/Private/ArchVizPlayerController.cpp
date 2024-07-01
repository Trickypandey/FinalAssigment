#include "ArchVizPlayerController.h"
#include "CeilingActor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "UiWidget.h"

AArchVizPlayerController::AArchVizPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentMode = EModes::RoadConstruction;
}



void AArchVizPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
}


void AArchVizPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AArchVizPlayerController::BeginPlay()
{
	Super::BeginPlay();

    SetShowMouseCursor(true);
    SetInputMode(InputMode);

   if (!UiWidgetInstance && UiWidgetClass)
    {
        UiWidgetInstance = CreateWidget<UUiWidget>(this, UiWidgetClass);
        if (UiWidgetInstance)
        {
            UiWidgetInstance->AddToViewport();
        }
    }
   if (IsValid(RoadConstructionModeRef)) {
       RoadConstructionMode = NewObject<URoadCreationMode>(this, RoadConstructionModeRef);
       RoadConstructionMode->InitParam(this);
       RoadConstructionMode->Setup();
       RoadConstructionMode->SetupInputMapping();
   }

	if (IsValid(BuildingConstructionModeRef)) {
        BuildingConstructionMode = NewObject<UBuildingCreationMode>(this, BuildingConstructionModeRef);
        BuildingConstructionMode->InitParam(this);
        BuildingConstructionMode->Setup();
        BuildingConstructionMode->SetupInputMapping();
   }
   if (IsValid(InteriorDesignModeRef)) {
       InteriorDesignMode = NewObject<UInteriorDesignMode>(this, InteriorDesignModeRef);
       InteriorDesignMode->InitParam(this);
       InteriorDesignMode->Setup();
       InteriorDesignMode->SetupInputMapping();
   }

   ModeChangeHandle(CurrentMode);
}


void AArchVizPlayerController::SetIsAddingDoor(bool DoorFlag)
{

    BuildingConstructionMode->WallCreationMode->bIsDoorAdding = DoorFlag;
}

void AArchVizPlayerController::ModeChangeHandle(EModes Mode)
{
    
	switch (Mode) {
		case EModes::WallCreation:
			SetArchVizMode(BuildingConstructionMode);
			break;
		case EModes::RoadConstruction:
            SetArchVizMode(RoadConstructionMode);
			
			break;
		case EModes::InteriorDesign:
            SetArchVizMode(InteriorDesignMode);
			break;
      
	}

		CurrentMode = Mode;
	
}

void AArchVizPlayerController::SaveGame()
{
	if (RoadConstructionMode)
	{
        RoadConstructionMode->SaveRoads();
	}

	if (BuildingConstructionMode)
	{
        BuildingConstructionMode->SaveBuildings();
	}
}

void AArchVizPlayerController::LoadGame()
{
    if (RoadConstructionMode)
    {
        RoadConstructionMode->LoadRoads();
    }

    if (BuildingConstructionMode)
    {
        BuildingConstructionMode->LoadBuildings();
    }

}

void AArchVizPlayerController::SetArchVizMode(UBaseMode* NewArchVizMode) {
    if (CurrentModeClass) {
        CurrentModeClass->ExitMode();
    }

    CurrentModeClass = NewArchVizMode;

    if (CurrentModeClass) {
        CurrentModeClass->EnterMode();
    }
}
