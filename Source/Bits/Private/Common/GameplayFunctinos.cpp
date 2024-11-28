#include "Common/GameplayFunctinos.h"

#include "EnhancedInputSubsystems.h"
#include "Components/Player/SequenceManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FirstPersonPlayerBase.h"

class UEnhancedInputLocalPlayerSubsystem;

static UInputMappingContext* SavedMappings = nullptr;

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
				Subsystem->ClearAllMappings();
				SavedMappings = InInputMapping;
				Subsystem->AddMappingContext(InInputMapping, 0);
			}
		}
	}
}

void UGameplayFunctinos::ClearMappingContext(const UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->ClearAllMappings();
			}
		}
	}
}

void UGameplayFunctinos::RestoreMappingContext(const UObject* WorldContextObject)
{
	if (WorldContextObject && SavedMappings)
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(SavedMappings, 0);
			}
		}
	}
}

void UGameplayFunctinos::PlaySubtitles(const UObject* WorldContextObject, const TArray<FSubtitleSetting>& Subtitles)
{
	UWorld* World = WorldContextObject->GetWorld();
	if (AFirstPersonPlayerBase* Player = Cast<AFirstPersonPlayerBase>(UGameplayStatics::GetPlayerPawn(World, 0)))
	{
		Player->SequenceManager->PlaySubtitles(Subtitles);
	}
}
