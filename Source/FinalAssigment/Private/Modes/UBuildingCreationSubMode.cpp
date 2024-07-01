// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/UBuildingCreationSubMode.h"

void UUBuildingCreationSubMode::InitParams(APlayerController* Controller) {
	PlayerController = Controller;
}

void UUBuildingCreationSubMode::SetMaterial(UMaterialInterface* Material)
{
	if (SelectedActor && SelectedActor->GetProceduralMeshComponent())
	{
		DynamicMaterial = Material;
		SelectedActor->GetProceduralMeshComponent()->SetMaterial(0, Material);
		
	}
}

ACubeActor* UUBuildingCreationSubMode::GetSelectedActor()
{
	if (SelectedActor)
	{
		return SelectedActor;
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
