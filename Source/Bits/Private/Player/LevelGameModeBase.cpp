#include "Player/LevelGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "Common/GameplayFunctinos.h"
#include "Common/SubtitleSetting.h"
#include "Components/Player/SequenceManagerComponent.h"
#include "HUD/CountdownHUDBase.h"
#include "Player/FirstPersonPlayerBase.h"

void ALevelGameModeBase::StopTime()
{
	if (CountdownWidget)
	{
		CountdownWidget->RemoveFromParent();
		CurrentTime = -10.f;
	}
}

void ALevelGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (!SubtitlesBeforeStartGame.IsEmpty())
	{
		float DelayStartTime = 0.f;
		for (auto& Text : SubtitlesBeforeStartGame)
		{
			DelayStartTime += Text.SubtitleDelay;
		}

		FTimerHandle StartGameTimerHandle;
		GetWorldTimerManager().SetTimer(
			StartGameTimerHandle,
			this,
			&ThisClass::StartGame,
			DelayStartTime,
			false
		);
		PlaySubtitles(SubtitlesBeforeStartGame);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::StartGame);
	}
}

void ALevelGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentTime > 0)
	{
		CurrentTime -= DeltaSeconds;
	}
	if (CurrentTime <= 0 && CurrentTime > -1.)
	{
	}
}

void ALevelGameModeBase::StartGame()
{
	CurrentTime = LevelTime;
	if (CountdownWidgetClass)
	{
		CountdownWidget = CreateWidget<UCountdownHUDBase>(GetWorld(), CountdownWidgetClass);
		if (CountdownWidget)
		{
			CountdownWidget->AddToViewport();
		}
		CountdownWidget->SetTime(LevelTime);
	}

	if (GetPlayer())
	{
		GetPlayer()->StartGame();
	}
}

void ALevelGameModeBase::PlaySubtitles(const TArray<FSubtitleSetting>& Subtitles)
{
	if (GetPlayer() && GetPlayer()->SequenceManager)
	{
		GetPlayer()->SequenceManager->PlaySubtitles(Subtitles);
	}
}

AFirstPersonPlayerBase* ALevelGameModeBase::GetPlayer()
{
	if (IsValid(CurrentPlayer))
	{
		return CurrentPlayer;
	}
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		CurrentPlayer = PlayerController->GetPawn<AFirstPersonPlayerBase>();
	}
	return CurrentPlayer;
}
