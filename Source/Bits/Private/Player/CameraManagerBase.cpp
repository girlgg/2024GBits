#include "Player/CameraManagerBase.h"

#include "Blueprint/UserWidget.h"
#include "HUD/GeneralHUDBase.h"

ACameraManagerBase::ACameraManagerBase() {}

void ACameraManagerBase::BeginPlay()
{
	if (GeneralHUDClass)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			GeneralHUD = CreateWidget<UGeneralHUDBase>(PlayerController, GeneralHUDClass);
			if (GeneralHUD)
			{
				GeneralHUD->AddToViewport();

				FInputModeGameOnly InputMode;
				PlayerController->SetInputMode(InputMode);
				PlayerController->bShowMouseCursor = false;
			}
		}
	}
}