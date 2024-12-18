#include "Player/CameraManagerBase.h"

#include "Blueprint/UserWidget.h"
#include "Common/ViewMode.h"
#include "HUD/GeneralHUDBase.h"

ACameraManagerBase::ACameraManagerBase()
{
}

void ACameraManagerBase::BeginPlay()
{
}

void ACameraManagerBase::StartGame()
{
	CreateCameraHUD();
}

void ACameraManagerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACameraManagerBase::ShowCameraHUD()
{
	if (GeneralHUD)
	{
		GeneralHUD->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACameraManagerBase::HideCameraHUD()
{
	if (GeneralHUD)
	{
		GeneralHUD->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ACameraManagerBase::CreateCameraHUD()
{
	if (GeneralHUD)
	{
		DestroyCameraHUD();
	}
	if (GeneralHUDClass)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			GeneralHUD = CreateWidget<UGeneralHUDBase>(PlayerController, GeneralHUDClass);
			if (GeneralHUD)
			{
				GeneralHUD->AddToViewport();
			}
		}
	}
}

void ACameraManagerBase::DestroyCameraHUD()
{
	GeneralHUD->RemoveFromParent();
	GeneralHUD = nullptr;
}
