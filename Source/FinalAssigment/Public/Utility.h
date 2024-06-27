#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBuildingCreationType : uint8
{
    Wall,
    Floor,
    Ceiling
};

UENUM(BlueprintType)
enum class EModes : uint8
{
  
    WallCreation UMETA(DisplayName = "Wall Creation Mode"),
    RoadConstruction UMETA(DisplayName = "Road Construction Mode"),
    InteriorDesign UMETA(DisplayName = "Actor Spawning Mode")

};

UENUM(BlueprintType)
enum class EBuildingSubModeState : uint8 {
	Placed,
	Moving
};

namespace Utility
{
	inline FVector SnapToGrid(const FVector& WorldLocation, const FVector& GridSize, bool bSnapZ = false) {
	    float SnappedX = FMath::RoundToFloat(WorldLocation.X / GridSize.X) * GridSize.X;
	    float SnappedY = FMath::RoundToFloat(WorldLocation.Y / GridSize.Y) * GridSize.Y;
	    float SnappedZ = bSnapZ ? FMath::RoundToFloat(WorldLocation.Z / GridSize.Z) * GridSize.Z : WorldLocation.Z;

	    return FVector(SnappedX, SnappedY, SnappedZ);
	}
}


