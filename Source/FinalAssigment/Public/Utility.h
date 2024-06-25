#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EObjectType : uint8
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

