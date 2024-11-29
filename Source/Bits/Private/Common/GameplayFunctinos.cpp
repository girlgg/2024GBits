#include "Common/GameplayFunctinos.h"

#include "EnhancedInputSubsystems.h"
#include "Components/Player/InventoryManagerComponent.h"
#include "Components/Player/SequenceManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BitGameInstanceBase.h"
#include "Player/FirstPersonPlayerBase.h"
#include "Player/LevelGameModeBase.h"

class UEnhancedInputLocalPlayerSubsystem;

static UInputMappingContext* SavedMappings = nullptr;
static UInputMappingContext* WantToSavedMappings = nullptr;

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
				Subsystem->AddMappingContext(InInputMapping, 0);
				
				SavedMappings = WantToSavedMappings;
				WantToSavedMappings = InInputMapping;
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
				
				Swap(SavedMappings, WantToSavedMappings);
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

void UGameplayFunctinos::AddItemToPlayerInventory(const UObject* WorldContextObject,
                                                  const FInteractiveData& InInteractiveData)
{
	UWorld* World = WorldContextObject->GetWorld();
	if (AFirstPersonPlayerBase* Player = Cast<AFirstPersonPlayerBase>(UGameplayStatics::GetPlayerPawn(World, 0)))
	{
		Player->InventoryManager->AddItemToInventory(InInteractiveData, nullptr);
	}
}

void UGameplayFunctinos::MakeGameWin(const UObject* WorldContextObject)
{
	UWorld* World = WorldContextObject->GetWorld();
	if (ALevelGameModeBase* GameMode = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(World)))
	{
		GameMode->WinGame();
	}
}

void UGameplayFunctinos::SetNextLevelName(const UObject* WorldContextObject, FString LevelName)
{
	UWorld* World = WorldContextObject->GetWorld();
	UBitGameInstanceBase* GameInstance =
		Cast<UBitGameInstanceBase>(UGameplayStatics::GetGameInstance(World));
	if (GameInstance)
	{
		GameInstance->LevelNameToLoad = LevelName;
	}
}

void UGameplayFunctinos::LevelTravel(const UObject* WorldContextObject)
{
	UWorld* World = WorldContextObject->GetWorld();
	UBitGameInstanceBase* GameInstance =
		Cast<UBitGameInstanceBase>(UGameplayStatics::GetGameInstance(World));
	if (GameInstance)
	{
		const FString LevelName = GameInstance->LevelNameToLoad;
		UGameplayStatics::OpenLevel(World, *LevelName);
	}
}

AFirstPersonPlayerBase* UGameplayFunctinos::GetPlayer(const UObject* WorldContextObject)
{
	AFirstPersonPlayerBase* Player =
		Cast<AFirstPersonPlayerBase>(UGameplayStatics::GetPlayerPawn(WorldContextObject, 0));
	return Player;
}
