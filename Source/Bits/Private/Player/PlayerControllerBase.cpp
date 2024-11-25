#include "Player/PlayerControllerBase.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Player/CameraManagerBase.h"

APlayerControllerBase::APlayerControllerBase()
{
	PlayerCameraManagerClass = ACameraManagerBase::StaticClass();
}

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
}
