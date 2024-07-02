#include "ArchVizPlayerController.h"
#include "CeilingActor.h"
#include "SaveGameWidget.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "UiWidget.h"
#include "Kismet/GameplayStatics.h"

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

	if (!UiWidgetInstance && UiWidgetClass)
    {
        UiWidgetInstance = CreateWidget<UUiWidget>(this, UiWidgetClass);
        if (UiWidgetInstance)
        {
            UiWidgetInstance->AddToViewport();
        }
    }

	if (!ToastMessageWidgetInstance && ToastMessageWidgetClass)
    {
        ToastMessageWidgetInstance = CreateWidget<UToastMessageWidget>(this, ToastMessageWidgetClass);
        if (ToastMessageWidgetInstance)
        {
            ToastMessageWidgetInstance->AddToViewport();
        }
    }


	if (!SaveWidgetInstance && SaveWidgetClass)
    {
        SaveWidgetInstance = CreateWidget<USaveGameWidget>(this, SaveWidgetClass);
    }

	if (!LoadSlotWidgetInstance && LoadSlotWidgetClass)
    {
        LoadSlotWidgetInstance = CreateWidget<ULoadSlotWidget>(this, LoadSlotWidgetClass);
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

    BuildingConstructionMode->WallCreationMode->SetIsDoorAddingFlag(DoorFlag) ;
}

void AArchVizPlayerController::ModeChangeHandle(EModes Mode)
{
    
	switch (Mode) {
		case EModes::BuildingCreation:
            ToastMessageWidgetInstance->ShowMessage.Broadcast("In Building Creation Mode");
			SetArchVizMode(BuildingConstructionMode);
			break;
		case EModes::RoadConstruction:
            ToastMessageWidgetInstance->ShowMessage.Broadcast("In Road Construction Mode");
            SetArchVizMode(RoadConstructionMode);
			
			break;
		case EModes::InteriorDesign:
            ToastMessageWidgetInstance->ShowMessage.Broadcast("In Interior Design Mode");
            SetArchVizMode(InteriorDesignMode);
			break;
      
	}

		CurrentMode = Mode;
	
}

bool AArchVizPlayerController::SaveGame(FString SlotName)
{
    UUArchVizSaveGame* SaveGameInstance = Cast<UUArchVizSaveGame>(UGameplayStatics::CreateSaveGameObject(UUArchVizSaveGame::StaticClass()));
    if (!SaveGameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Fail TO create SaveGameInstance in Controller"));
        return false;
    }

	if (RoadConstructionMode)
	{
        RoadConstructionMode->SaveRoads(SaveGameInstance);
	}

	if (BuildingConstructionMode)
	{
        BuildingConstructionMode->SaveBuildings(SaveGameInstance);
	}


    bool bSaveSuccessful = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);
    if (bSaveSuccessful)
    {
        UE_LOG(LogTemp, Log, TEXT("Game with road data successfully saved to slot"));
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save game with road data to slot"));
        return false;
    }


}

void AArchVizPlayerController::LoadGame(FString SlotName)
{
    UUArchVizSaveGame* LoadGameInstance = Cast<UUArchVizSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

    if (!LoadGameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Fail TO create LoadGameInstance in Controller" ));
        return;
    }

    if (RoadConstructionMode)
    {
        RoadConstructionMode->LoadRoads(LoadGameInstance);
    }

    if (BuildingConstructionMode)
    {
        BuildingConstructionMode->LoadBuildings(LoadGameInstance);
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

void AArchVizPlayerController::BroadcastToast(const FString& Message)
{
	if (ToastMessageWidgetInstance)
	{
		ToastMessageWidgetInstance->ShowMessage.Broadcast(Message);
	}
}
