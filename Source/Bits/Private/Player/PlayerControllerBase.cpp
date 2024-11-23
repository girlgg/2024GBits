#include "Player/PlayerControllerBase.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	// 初始化输入上下文
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InputMappingContext)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}
