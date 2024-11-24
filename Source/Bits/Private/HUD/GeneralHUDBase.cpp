#include "HUD/GeneralHUDBase.h"

#include "Animation/WidgetAnimation.h"
#include "Components/Player/InteractionManagerComponent.h"
#include "Player/FirstPersonPlayerBase.h"

void UGeneralHUDBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (AFirstPersonPlayerBase* Player = Cast<AFirstPersonPlayerBase>(GetOwningPlayer()->GetPawn()))
	{
		if (Player->InteractionManager)
		{
			bool bCanInteract = Player->InteractionManager->CanInteract();

			if (bCanInteract != bLastCanInteract && IsValid(CanInteractCursorAnim))
			{
				if (bCanInteract)
				{
					PlayAnimation(CanInteractCursorAnim, 0, 1, EUMGSequencePlayMode::Forward, 1.5f);
				}
				else
				{
					PlayAnimation(CanInteractCursorAnim, 0, 1, EUMGSequencePlayMode::Reverse, 1.5f);
				}
				bLastCanInteract = bCanInteract;
			}
		}
	}
}
