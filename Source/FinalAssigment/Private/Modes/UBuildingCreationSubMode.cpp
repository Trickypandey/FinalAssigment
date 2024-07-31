// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/UBuildingCreationSubMode.h"
#include "ArchVizPlayerController.h"
#include "SlabActor.h"

void UUBuildingCreationSubMode::InitParams(APlayerController* Controller) {
	PlayerController = Controller;
}

void UUBuildingCreationSubMode::SetMaterial(UMaterialInterface* Material)
{
	if (Material)
	{
		DynamicMaterial = Material;

		if(auto actor = Cast<AWallActor>(SelectedActor))
		{
			
			actor->SetMaterial(DynamicMaterial);
		}

		if(auto actor = Cast<ASlabActor>(SelectedActor))
		{
			
			actor->SetMaterial(DynamicMaterial);
		}
		
	}
}

ACubeActor* UUBuildingCreationSubMode::GetSelectedActor()
{
	if (SelectedActor)
	{
		return Cast<ACubeActor>(SelectedActor) ;
	}
	return nullptr;
}

FHitResult UUBuildingCreationSubMode::GetHitResult(const TArray<AActor*>& ActorsToIgnore) const {
	FHitResult HitResult{};

	FVector WorldLocation{}, WorldDirection{};

	if (IsValid(PlayerController) && PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {
		FVector TraceStart = WorldLocation;
		FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bTraceComplex = true;
		CollisionQueryParams.AddIgnoredActor(PlayerController->GetPawn());
		CollisionQueryParams.AddIgnoredActors(ActorsToIgnore);

		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);
	}

	return HitResult;
}
