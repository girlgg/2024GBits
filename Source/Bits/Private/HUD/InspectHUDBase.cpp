#include "HUD/InspectHUDBase.h"

#include "Components/TextBlock.h"

void UInspectHUDBase::UpdateReadText(const TArray<FString>& InPages)
{
	Pages = InPages;
	CurrentPage = 0;
	UpdatePageContent(Pages[0]);
}

void UInspectHUDBase::NavigateReadablePages(float Direction)
{
	if (Direction > 0)
	{
		++CurrentPage;
	}
	else
	{
		--CurrentPage;
	}
	CurrentPage = FMath::Clamp(CurrentPage, 0, Pages.Num() - 1);
	UpdatePageContent(Pages[CurrentPage]);
}

void UInspectHUDBase::UpdatePageContent(FString InText)
{
	TextGuide->SetText(FText::FromString(InText));
}
