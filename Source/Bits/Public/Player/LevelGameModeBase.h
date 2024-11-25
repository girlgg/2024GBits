#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LevelGameModeBase.generated.h"

class AFirstPersonPlayerBase;
struct FSubtitleSetting;
class UCountdownHUDBase;

UCLASS()
class BITS_API ALevelGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	void StopTime();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void StartGame();

	UFUNCTION(BlueprintCallable)
	void PlaySubtitles(const TArray<FSubtitleSetting>& Subtitles);

	AFirstPersonPlayerBase* GetPlayer();

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UCountdownHUDBase> CountdownWidgetClass;
	UPROPERTY(Transient, BlueprintReadOnly)
	UCountdownHUDBase* CountdownWidget;

	/* 本关开始前对话 */
	UPROPERTY(EditDefaultsOnly, Category = "Level Config")
	TArray<FSubtitleSetting> SubtitlesBeforeStartGame;

	/* 本关倒计时 */
	UPROPERTY(EditDefaultsOnly, Category = "Level Config")
	float LevelTime{0.f};

private:
	UPROPERTY(Transient)
	AFirstPersonPlayerBase* CurrentPlayer;

	float CurrentTime;
};
