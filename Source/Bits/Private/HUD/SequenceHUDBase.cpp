#include "HUD/SequenceHUDBase.h"

#include "Common/SubtitleSetting.h"
#include "Components/TextBlock.h"

void USequenceHUDBase::DisplaySubtitles(const TArray<FSubtitleSetting>& InSubtitles)
{
	SubtitleText->SetText(FText());
	Subtitles = InSubtitles;
	CurrentSubtitleIndex = 0;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	UpdateSubtitles();
}

void USequenceHUDBase::UpdateSubtitles()
{
	if (CurrentSubtitleIndex < Subtitles.Num())
	{
		SubtitleText->SetText(FText::FromString(Subtitles[CurrentSubtitleIndex].SubtitleText));
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::PlaySubtitle,
		                                       Subtitles[CurrentSubtitleIndex].SubtitleDelay, false);
	}
	else
	{
		Subtitles.Empty();
		CurrentSubtitleIndex = 0;
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void USequenceHUDBase::PlaySubtitle()
{
	++CurrentSubtitleIndex;
	UpdateSubtitles();
}
