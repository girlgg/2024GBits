#include "HUD/SequenceHUDBase.h"

#include "Common/SubtitleSetting.h"
#include "Components/TextBlock.h"

void USequenceHUDBase::DisplaySubtitles(const TArray<FSubtitleSetting>& InSubtitles)
{
	SubtitleText->SetText(FText());
	Subtitles = InSubtitles;
	CurrentSubtitleIndex = 0;
	AddToViewport();
	UpdateSubtitles();
}

void USequenceHUDBase::UpdateSubtitles()
{
	if (CurrentSubtitleIndex < Subtitles.Num())
	{
		SubtitleText->SetText(FText::FromString(Subtitles[CurrentSubtitleIndex].SubtitleText));
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::PlaySubtitle,
		                                       Subtitles[CurrentSubtitleIndex].SubtitleDelay, false);
	}
	else
	{
		Subtitles.Empty();
		CurrentSubtitleIndex = 0;
		RemoveFromParent();
	}
}

void USequenceHUDBase::PlaySubtitle()
{
	++CurrentSubtitleIndex;
	UpdateSubtitles();
}
