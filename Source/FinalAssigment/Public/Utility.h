#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EObjectType : uint8
{
    Wall,
    Slab,
    Door,
    Window,
    Chair,
    Table
};