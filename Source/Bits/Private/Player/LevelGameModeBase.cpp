#include "Player/LevelGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "Common/GameplayFunctinos.h"
#include "Common/SubtitleSetting.h"
#include "Components/Player/InventoryManagerComponent.h"
#include "Components/Player/SequenceManagerComponent.h"
#include "HUD/CountdownHUDBase.h"
#include "HUD/StartGameHUDBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FirstPersonPlayerBase.h"

void ALevelGameModeBase::IntoDream(float InDreamTime)
{
	CurrentSecondsCount += InDreamTime + GetDreamExtraTime();

	if (CountdownWidget)
	{
		CountdownWidget->SetVisibility(ESlateVisibility::Hidden);
		CountdownWidget->SetCurrentTime(CurrentTime);
		bInDream = true;
		CountdownWidget->SetPause(bInDream);
		CountdownWidget->SetSleepTime(CurrentSecondsCount);
	}

	OnIntoDream.Broadcast();
}

void ALevelGameModeBase::OutDream()
{
	if (CountdownWidget)
	{
		CountdownWidget->SetVisibility(ESlateVisibility::Visible);
		bInDream = false;
		CountdownWidget->SetPause(bInDream);
	}

	OnOutDream.Broadcast();
}

void ALevelGameModeBase::WinGame()
{
}

void ALevelGameModeBase::LoseGame()
{
	const FName NextLevelName = "GameOverLevel";
	UGameplayStatics::OpenLevel(this, NextLevelName);
}

void ALevelGameModeBase::ReduceTime(float InTime)
{
	CurrentTime -= InTime;
	CurrentTime = CurrentTime < 0 ? 0 : CurrentTime;
}

void ALevelGameModeBase::PauseGame()
{
	bIsPause = true;
	CountdownWidget->SetPause(true);
}

void ALevelGameModeBase::ContinueGame()
{
	bIsPause = false;
	CountdownWidget->SetPause(bInDream);
}

const FRoomConfig& ALevelGameModeBase::GetCurrentRoomPos(const FVector& PlayerPos) const
{
	for (const auto& Room : RoomInfo)
	{
		if (PlayerPos.X >= Room.ThirdPos.X - Room.Width &&
			PlayerPos.X <= Room.ThirdPos.X + Room.Width &&
			PlayerPos.Y >= Room.ThirdPos.Y - Room.Height &&
			PlayerPos.Y <= Room.ThirdPos.Y + Room.Height)
		{
			return Room;
		}
	}
	return RoomInfo[0];
}

void ALevelGameModeBase::ChangePlayerPos(const FVector& PlayerPos)
{
	if (GetPlayer())
	{
		GetPlayer()->SetActorLocation(PlayerPos);
	}
}

void ALevelGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentTime = LevelTime;
	bIsPause = true;
	ShowLevelTitle();
}

void ALevelGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsPause)
	{
		return;
	}
	if (!bInDream)
	{
		if (CurrentTime > 0)
		{
			CurrentTime -= DeltaSeconds;
		}
		else if (CurrentTime <= 0 && CurrentTime > -1.)
		{
			if (CurrentSecondsCount >= WinSeconds)
			{
				// 通关
				WinGame();
			}
			else
			{
				// 失败
				LoseGame();
			}
		}
	}
	else
	{
		CurrentDreamTime += DeltaSeconds;
		if (CurrentDreamTime >= DreamMaxTime)
		{
			// 失败
			LoseGame();
		}
	}
}

void ALevelGameModeBase::ShowLevelTitle()
{
	float DelayStartTime = ShowTitle(LevelName, SubtitlesBeforeStartGame);

	if (!SubtitlesBeforeStartGame.IsEmpty())
	{
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
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::StartGame);
	}
}

float ALevelGameModeBase::ShowTitle(const FString& InText, const TArray<FSubtitleSetting>& TextArr, bool bInPause)
{
	if (GetPlayer() && bInPause)
	{
		GetPlayer()->BePause();
	}
	if (StartGameWidget)
	{
		StartGameWidget->RemoveFromParent();
		StartGameWidget = nullptr;
	}
	ArrToShow = TextArr;
	if (StartGameWidgetClass)
	{
		StartGameWidget = CreateWidget<UStartGameHUDBase>(GetWorld(), StartGameWidgetClass);
		if (StartGameWidget)
		{
			StartGameWidget->AddToViewport();
		}
		StartGameWidget->UpdateTitleName(InText);
		float TitleTime = StartGameWidget->PlayHideAnim();
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::StartGameDialog, TitleTime, false);
		return TitleTime;
	}
	return 0.f;
}

void ALevelGameModeBase::StartGameDialog()
{
	if (StartGameWidget)
	{
		StartGameWidget->RemoveFromParent();
	}
	PlaySubtitles(ArrToShow);
}

void ALevelGameModeBase::StartGame()
{
	bIsPause = false;
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
	float WaitTime = 0.f;
	for (const FSubtitleSetting& Subtitle : Subtitles)
	{
		WaitTime += Subtitle.SubtitleDelay;
	}
	GetWorld()->GetTimerManager().SetTimer(SubtitleTimerHandle, [this]()
	{
		if (GetPlayer())
		{
			GetPlayer()->OutPause();
		}
	}, WaitTime, false);
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

float ALevelGameModeBase::GetDreamExtraTime()
{
	float RandomFloat = FMath::RandRange(0.0f, 1.0f);
	float CurrentCnt = 0;
	for (auto& Effect : DreamEffects)
	{
		if (RandomFloat <= CurrentCnt)
		{
			if (Effect.CurrentTimes >= Effect.Times)
			{
				return GetDreamExtraTime();
			}
			++Effect.CurrentTimes;
			return Effect.AddTime;
		}
		CurrentCnt += Effect.Probability;
	}
	return 0.f;
}
