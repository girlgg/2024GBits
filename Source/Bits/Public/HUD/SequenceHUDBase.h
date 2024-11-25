#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SequenceHUDBase.generated.h"

class UTextBlock;
struct FSubtitleSetting;

UCLASS()
class BITS_API USequenceHUDBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void DisplaySubtitles(const TArray<FSubtitleSetting>& InSubtitles);
	void UpdateSubtitles();
	void PlaySubtitle();

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SubtitleText;

private:
	TArray<FSubtitleSetting> Subtitles;
	int32 CurrentSubtitleIndex{0};
};
