#include "HUD/StartGameHUDBase.h"

#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"

void UStartGameHUDBase::UpdateTitleName(const FString& InTitle)
{
	LevelTitle->SetText(FText::FromString(InTitle));
}

float UStartGameHUDBase::PlayHideAnim()
{
	PlayAnimation(HideAnim, 0, 1, EUMGSequencePlayMode::Forward, 1.f);
	return HideAnim->GetEndTime() - HideAnim->GetStartTime();
}
