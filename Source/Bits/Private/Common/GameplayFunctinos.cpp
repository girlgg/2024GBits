#include "Common/GameplayFunctinos.h"

#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

class UEnhancedInputLocalPlayerSubsystem;

void UGameplayFunctinos::UpdateInputMappingContext(const UObject* WorldContextObject,
                                                   UInputMappingContext* InInputMapping)
{
	if (WorldContextObject && InInputMapping)
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				if (InInputMapping)
				{
					Subsystem->ClearAllMappings();
					Subsystem->AddMappingContext(InInputMapping, 0);
				}
			}
		}
	}
}
