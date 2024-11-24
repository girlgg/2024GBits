#include "HUD/ItemInteractionHUDBase.h"

#include "Components/TextBlock.h"

void UItemInteractionHUDBase::UpdateInputInteractionData(FInteractiveData& InInteractiveData)
{
	InteractiveData = InInteractiveData;
	PromptText->SetText(FText::FromString(InteractiveData.InteractionMethod.PromptText));
}
