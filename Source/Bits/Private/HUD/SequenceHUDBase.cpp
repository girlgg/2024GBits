#include "HUD/SequenceHUDBase.h"

#include "Common/GameplayFunctinos.h"
#include "Common/SubtitleSetting.h"
#include "Components/TextBlock.h"
#include "Components/Player/SequenceManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FirstPersonPlayerBase.h"

void USequenceHUDBase::NativeConstruct()
{
	Super::NativeConstruct();
	BackgroundImage = BackgroundT;
}

void USequenceHUDBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void USequenceHUDBase::DisplaySubtitles(const TArray<FSubtitleSetting>& InSubtitles)
{
	TextToShow = FString("");
	Subtitles = InSubtitles;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(TextTimerHandle);

	UpdateSubtitles();
}

void USequenceHUDBase::UpdateSubtitles()
{
	if (CurrentSubtitleIndex < Subtitles.Num())
	{
		CurrentText = Subtitles[CurrentSubtitleIndex].SubtitleText;
		CurrentDelay = Subtitles[CurrentSubtitleIndex].SubtitleDelay;

		if (!CurrentText.IsEmpty())
		{
			CurrentTextIdx = 0;
			TextToShow = FString(1, &CurrentText[CurrentTextIdx++]);
			CurrentTextDelay = CurrentDelay / CurrentText.Len() / 2.;

			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::PlaySubtitle, CurrentDelay, false);
			GetWorld()->GetTimerManager().SetTimer(TextTimerHandle, this, &ThisClass::UpdateText, CurrentTextDelay,
			                                       false);
			return;
		}
	}
	Subtitles.Empty();
	CurrentSubtitleIndex = 0;
	SetVisibility(ESlateVisibility::Hidden);
	UGameplayFunctinos::GetPlayer(GetWorld())->SequenceManager->OnSubtitlesFinished.ExecuteIfBound();
}

void USequenceHUDBase::UpdateText()
{
	if (CurrentTextIdx >= CurrentText.Len())
	{
		return;
	}
	TextToShow += FString(1, &CurrentText[CurrentTextIdx++]);
	GetWorld()->GetTimerManager().SetTimer(TextTimerHandle, this, &ThisClass::UpdateText, CurrentTextDelay, false);
}

void USequenceHUDBase::PlaySubtitle()
{
	++CurrentSubtitleIndex;
	UpdateSubtitles();
}

void USequenceHUDBase::IntoDream()
{
	BackgroundImage = BackgroundD;
}

void USequenceHUDBase::OutDream()
{
	BackgroundImage = BackgroundT;
}
