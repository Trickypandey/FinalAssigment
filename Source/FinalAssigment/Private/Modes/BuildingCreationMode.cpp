// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/BuildingCreationMode.h"
#include "AssertManager.h"
#include "CeilingActor.h"
#include "FloorActor.h"
#include "InteriorDesignActor.h"
#include "SlabActor.h"
#include "UArchVizSaveGame.h"
#include "WallConstructionWidget.h"
#include "Kismet/GameplayStatics.h"

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
    CleanUp();
}

void UBuildingCreationMode::CleanUp()
{
	if (CurrentBuildingCreationSubMode)
	{
        CurrentBuildingCreationSubMode->ExitSubMode(Cast<UWallConstructionWidget>(Widget));
	}
    CurrentBuildingCreationSubMode = nullptr;
}

ACubeActor* UBuildingCreationMode::GetSelectedActor()
{
	if (CurrentBuildingCreationSubMode)
	{
		return CurrentBuildingCreationSubMode->GetSelectedActor();
	}
    return  nullptr;
}



void UBuildingCreationMode::SpawnSelectedActor(EBuildingCreationType Type)
{
    // Declare the variable outside the switch statement
    UWallConstructionWidget* WallConstructionWidget = nullptr;

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
 
    if (!MeshData.Type)
    {
        UE_LOG(LogTemp, Error, TEXT("BaseMaterial is nullptr in AArchVizPlayerController"));
    }

    if (MeshData.Type)
    {
	    if (CurrentBuildingCreationSubMode)
	    {
    		CurrentBuildingCreationSubMode->SetMaterial(MeshData.Type);
	    }
    }
}


void UBuildingCreationMode::SaveBuildings(UUArchVizSaveGame*& SaveGameInstance)
{
    if (!SaveGameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create save game object"));
        return;
    }

    // Collect all Wall, Floor, and Ceiling actors
    TArray<AActor*> WallActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWallActor::StaticClass(), WallActors);

    TArray<AActor*> FloorActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFloorActor::StaticClass(), FloorActors);

    TArray<AActor*> CeilingActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACeilingActor::StaticClass(), CeilingActors);

    // Save Wall Actors
    for (AActor* Actor : WallActors)
    {
        if (AWallActor* WallActor = Cast<AWallActor>(Actor))
        {
            FBuildingActorData ActorData;
            ActorData.ActorClass = WallActor->GetClass();
            ActorData.ActorTransform = WallActor->GetActorTransform();
            //ActorData.ActorName = WallActor->GetName();
            ActorData.bIsDoorAdded = WallActor->GetDoorFlag();

            // Save attached interior actors
            TArray<AActor*> AttachedActors;
            WallActor->GetAttachedActors(AttachedActors);
            for (AActor* AttachedActor : AttachedActors)
            {
                if (AInteriorDesignActor* InteriorActor = Cast<AInteriorDesignActor>(AttachedActor))
                {
                    FInteriorActorData InteriorData;
                    InteriorData.ActorClass = InteriorActor->GetClass();
                    InteriorData.ActorTransform = InteriorActor->GetActorTransform();
          /*          InteriorData.AttachedActorName = ActorData.ActorName;
                    InteriorData.ActorName = InteriorActor->GetName();*/

                    // Save static mesh
                    if (UStaticMeshComponent* StaticMeshComp = InteriorActor->FindComponentByClass<UStaticMeshComponent>())
                    {
                        if (StaticMeshComp->GetStaticMesh())
                        {
                            InteriorData.StaticMeshPath = StaticMeshComp->GetStaticMesh()->GetPathName();
                        }
                    }

                    ActorData.AttachedInteriorActors.Add(InteriorData);
                }
            }

            SaveGameInstance->SaveData.WallActors.Add(ActorData);
        }
    }

    // Repeat the same process for Floor and Ceiling Actors
    for (AActor* Actor : FloorActors)
    {
        if (AFloorActor* FloorActor = Cast<AFloorActor>(Actor))
        {
            FBuildingActorData ActorData;
            ActorData.ActorClass = FloorActor->GetClass();
            ActorData.ActorTransform = FloorActor->GetActorTransform();
            //ActorData.ActorName = FloorActor->GetName();

            // Save attached interior actors
            TArray<AActor*> AttachedActors;
            FloorActor->GetAttachedActors(AttachedActors);
            for (AActor* AttachedActor : AttachedActors)
            {
                if (AInteriorDesignActor* InteriorActor = Cast<AInteriorDesignActor>(AttachedActor))
                {
                    FInteriorActorData InteriorData;
                    InteriorData.ActorClass = InteriorActor->GetClass();
                    InteriorData.ActorTransform = InteriorActor->GetActorTransform();
                   /* InteriorData.AttachedActorName = ActorData.ActorName;
                    InteriorData.ActorName = InteriorActor->GetName();*/

                    // Save static mesh
                    if (UStaticMeshComponent* StaticMeshComp = InteriorActor->FindComponentByClass<UStaticMeshComponent>())
                    {
                        if (StaticMeshComp->GetStaticMesh())
                        {
                            InteriorData.StaticMeshPath = StaticMeshComp->GetStaticMesh()->GetPathName();
                        }
                    }

                    ActorData.AttachedInteriorActors.Add(InteriorData);
                }
            }

            SaveGameInstance->SaveData.FloorActors.Add(ActorData);
        }
    }

    for (AActor* Actor : CeilingActors)
    {
        if (ACeilingActor* CeilingActor = Cast<ACeilingActor>(Actor))
        {
            FBuildingActorData ActorData;
            ActorData.ActorClass = CeilingActor->GetClass();
            ActorData.ActorTransform = CeilingActor->GetActorTransform();
            //ActorData.ActorName = CeilingActor->GetName();

            // Save attached interior actors
            TArray<AActor*> AttachedActors;
            CeilingActor->GetAttachedActors(AttachedActors);
            for (AActor* AttachedActor : AttachedActors)
            {
                if (AInteriorDesignActor* InteriorActor = Cast<AInteriorDesignActor>(AttachedActor))
                {
                    FInteriorActorData InteriorData;
                    InteriorData.ActorClass = InteriorActor->GetClass();
                    InteriorData.ActorTransform = InteriorActor->GetActorTransform();
                   /* InteriorData.AttachedActorName = ActorData.ActorName;
                    InteriorData.ActorName = InteriorActor->GetName();*/

                    // Save static mesh
                    if (UStaticMeshComponent* StaticMeshComp = InteriorActor->FindComponentByClass<UStaticMeshComponent>())
                    {
                        if (StaticMeshComp->GetStaticMesh())
                        {
                            InteriorData.StaticMeshPath = StaticMeshComp->GetStaticMesh()->GetPathName();
                        }
                    }

                    ActorData.AttachedInteriorActors.Add(InteriorData);
                }
            }

            SaveGameInstance->SaveData.CeilingActors.Add(ActorData);
        }
    }
}




void UBuildingCreationMode::LoadBuildings(UUArchVizSaveGame*& LoadGameInstance)
{
    if (!LoadGameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load game from slot"));
        return;
    }

    // Clear existing actors if necessary
    TArray<AActor*> ExistingActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACubeActor::StaticClass(), ExistingActors);
    for (AActor* Actor : ExistingActors)
    {
	    if (Cast<ACubeActor>(Actor))
	    {
			Actor->Destroy();
            Actor = nullptr;
	    }

    }

    // Helper function to spawn actors
    auto SpawnBuildingActor = [this](const FBuildingActorData& ActorData)
        {
            FActorSpawnParameters SpawnParams;
            /*SpawnParams.Name = FName(*ActorData.ActorName);*/

            AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorData.ActorClass, ActorData.ActorTransform, SpawnParams);
            if (auto WallActor = Cast<AWallActor>(SpawnedActor))
            {
                WallActor->SetIsDoorAdded(ActorData.bIsDoorAdded);
            }

            if (SpawnedActor)
            {
                UE_LOG(LogTemp, Log, TEXT("Actor %s spawned successfully"), *SpawnedActor->GetName());

                // Spawn and attach interior actors
                for (const FInteriorActorData& InteriorData : ActorData.AttachedInteriorActors)
                {
                    FActorSpawnParameters InteriorSpawnParams;
                    /*InteriorSpawnParams.Name = FName(*InteriorData.ActorName);*/

                    AInteriorDesignActor* InteriorActor = GetWorld()->SpawnActor<AInteriorDesignActor>(InteriorData.ActorClass, InteriorData.ActorTransform, InteriorSpawnParams);
                    if (InteriorActor)
                    {
                        InteriorActor->AttachToActor(SpawnedActor, FAttachmentTransformRules::KeepWorldTransform);

                        // Load static mesh
                        if (!InteriorData.StaticMeshPath.IsEmpty())
                        {
                            UStaticMesh* StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *InteriorData.StaticMeshPath));
                            if (StaticMesh)
                            {
                                if (UStaticMeshComponent* StaticMeshComp = InteriorActor->FindComponentByClass<UStaticMeshComponent>())
                                {
                                    StaticMeshComp->SetStaticMesh(StaticMesh);
                                }
                            }
                        }
                    }
                }
            }
        };

    // Spawn Wall Actors
    for (const FBuildingActorData& ActorData : LoadGameInstance->SaveData.WallActors)
    {
        SpawnBuildingActor(ActorData);
    }

    // Spawn Floor Actors
    for (const FBuildingActorData& ActorData : LoadGameInstance->SaveData.FloorActors)
    {
        SpawnBuildingActor(ActorData);
    }

    // Spawn Ceiling Actors
    for (const FBuildingActorData& ActorData : LoadGameInstance->SaveData.CeilingActors)
    {
        SpawnBuildingActor(ActorData);
    }
}


