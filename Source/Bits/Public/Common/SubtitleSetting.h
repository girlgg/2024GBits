#pragma once

#include "SubtitleSetting.generated.h"

USTRUCT(BlueprintType)
struct FSubtitleSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SubtitleText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SubtitleDelay{1.f};
};
