#pragma once

#include "ViewMode.generated.h"

UENUM(BlueprintType)
enum class EViewMode : uint8
{
	FirstPerson,
	ThirdPerson,
	Max
};
