#pragma once

#include "SubtitleSetting.generated.h"

USTRUCT(BlueprintType)
struct FSubtitleSetting
{
	GENERATED_BODY()

	/* 对话文字 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SubtitleText;
	/* 该行文字显示时长 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SubtitleDelay{1.f};
};
