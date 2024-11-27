#include "Player/LevelGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "Common/GameplayFunctinos.h"
#include "Common/SubtitleSetting.h"
#include "Components/Player/SequenceManagerComponent.h"
#include "HUD/CountdownHUDBase.h"
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
}

void ALevelGameModeBase::ReduceTime(float InTime)
{
	CurrentTime -= InTime;
	CurrentTime = CurrentTime < 0 ? 0 : CurrentTime;
}

FVector ALevelGameModeBase::GetCurrentRoomPos(const FVector& PlayerPos) const
{
	for (auto [ThirdPos, Width, Height] : RoomInfo)
	{
		if (PlayerPos.X <= ThirdPos.X - Width && PlayerPos.X >= ThirdPos.X + Width
			&& PlayerPos.Y <= ThirdPos.Y - Height && PlayerPos.Y >= ThirdPos.Y + Height)
		{
			return ThirdPos;
		}
	}
	return FVector(0, 0, 500);
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

	if (!bInDream)
	{
		if (CurrentTime > 0)
		{
			CurrentTime -= DeltaSeconds;
		}
		if (CurrentTime <= 0 && CurrentTime > -1.)
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
