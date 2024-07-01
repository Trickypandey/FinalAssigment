// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorActor.h"

void AFloorActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
    switch (WallState) {
	    case EBuildingSubModeState::Placed:
	        HandlePlacedState();
	        break;
	    case EBuildingSubModeState::Moving:
	        HandleMovingState();
	        break;
    }
}

AFloorActor::AFloorActor()
{
	ConstructionType = EBuildingCreationType::Floor;
}

void AFloorActor::HandlePlacedState()
{
    
}

void AFloorActor::HandleMovingState()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {

        FHitResult HitResult;
        FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, this);

        FVector CursorWorldLocation;
        FVector CursorWorldDirection;
        PlayerController->DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

        if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
        {
            FVector NewLocation = HitResult.Location;
            FVector SnappedLocation = Utility::SnapToGrid(NewLocation, FVector(10));
            SetActorLocation(SnappedLocation);
        }

    }
}
