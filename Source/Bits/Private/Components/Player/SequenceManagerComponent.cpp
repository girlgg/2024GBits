#include "Components/Player/SequenceManagerComponent.h"

#include "HUD/SequenceHUDBase.h"


USequenceManagerComponent::USequenceManagerComponent()
{
}

void USequenceManagerComponent::PlaySubtitles(const TArray<FSubtitleSetting>& Subtitles)
{
	if (!IsValid(SequenceHUD))
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			SequenceHUD = CreateWidget<USequenceHUDBase>(PlayerController, SequenceHUDClass);
			SequenceHUD->AddToViewport();
		}
	}
	if (IsValid(SequenceHUD))
	{
		SequenceHUD->SetVisibility(ESlateVisibility::Visible);
		SequenceHUD->DisplaySubtitles(Subtitles);
	}
}

void USequenceManagerComponent::IntoDream()
{
	if (SequenceHUD)
	{
		SequenceHUD->IntoDream();
	}
}


void USequenceManagerComponent::OutDream()
{
	if (SequenceHUD)
	{
		SequenceHUD->OutDream();
	}
}

void USequenceManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}
