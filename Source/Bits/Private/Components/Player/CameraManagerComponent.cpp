#include "Components/Player/CameraManagerComponent.h"

#include "Common/ViewMode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CameraManagerBase.h"


UCameraManagerComponent::UCameraManagerComponent()
{
}

void UCameraManagerComponent::StartGame()
{
	GetCameraManager()->StartGame();
}

void UCameraManagerComponent::ShowCameraHUD()
{
	if (GetCameraManager())
	{
		GetCameraManager()->ShowCameraHUD();
	}
}

void UCameraManagerComponent::HideCameraHUD()
{
	if (GetCameraManager())
	{
		GetCameraManager()->HideCameraHUD();
	}
}

void UCameraManagerComponent::CreateCameraHUD()
{
	if (GetCameraManager())
	{
		GetCameraManager()->CreateCameraHUD();
	}
}

void UCameraManagerComponent::DestroyCameraHUD()
{
	if (GetCameraManager())
	{
		GetCameraManager()->DestroyCameraHUD();
	}
}

void UCameraManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

ACameraManagerBase* UCameraManagerComponent::GetCameraManager()
{
	if (IsValid(CameraManager))
		return CameraManager;
	return CameraManager =
		Cast<ACameraManagerBase>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
}
