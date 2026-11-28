#pragma once

#include "TurnInPlaceEnum.generated.h"

UENUM(BlueprintType)
enum class ETurnInPlaceEnum : uint8
{
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
	NoTurning UMETA(DisplayName = "NoTurning")
};