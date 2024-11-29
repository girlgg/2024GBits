#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LevelGameModeBase.generated.h"

struct FInteractiveData;
class UStartGameHUDBase;
class AFirstPersonPlayerBase;
struct FSubtitleSetting;
class UCountdownHUDBase;

USTRUCT(BlueprintType)
struct FRoomConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FVector ThirdPos;
	UPROPERTY(EditDefaultsOnly)
	FRotator ThirdRotation{-90, 180, 0};
	UPROPERTY(EditDefaultsOnly)
	float Width;
	UPROPERTY(EditDefaultsOnly)
	float Height;
};

USTRUCT(BlueprintType)
struct FDreamEffect
{
	GENERATED_BODY()

	/* 进入梦的效果名 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EffectTime;
	/* 进入梦额外添加的时间 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AddTime;
	/* 进入该效果的概率 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Probability;
	/* 多少次后不再进入该效果，0为不使用 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Times{0};

	int32 CurrentTimes{0};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIntoDream);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutDream);

UCLASS()
class BITS_API ALevelGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void IntoDream(float InDreamTime);
	UFUNCTION(BlueprintCallable)
	void OutDream();

	UFUNCTION(BlueprintCallable)
	void WinGame();
	UFUNCTION(BlueprintCallable)
	void LoseGame();

	UFUNCTION(BlueprintCallable)
	void ReduceTime(float InTime);

	UFUNCTION(BlueprintCallable)
	void PauseGame();
	UFUNCTION(BlueprintCallable)
	void ContinueGame();

	const FRoomConfig& GetCurrentRoomPos(const FVector& PlayerPos) const;

	void ChangePlayerPos(const FVector& PlayerPos);

	UPROPERTY(BlueprintAssignable)
	FOnIntoDream OnIntoDream;
	UPROPERTY(BlueprintAssignable)
	FOnOutDream OnOutDream;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void ShowLevelTitle();
	UFUNCTION(BlueprintCallable)
	float ShowTitle(const FString& InText, const TArray<FSubtitleSetting>& TextArr, bool bInPause = false);
	void StartGameDialog();
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void PlaySubtitles(const TArray<FSubtitleSetting>& Subtitles);

	AFirstPersonPlayerBase* GetPlayer();

	float GetDreamExtraTime();

	/* 倒计时HUD控件 */
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UCountdownHUDBase> CountdownWidgetClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UStartGameHUDBase> StartGameWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly)
	UCountdownHUDBase* CountdownWidget;
	UPROPERTY(Transient, BlueprintReadOnly)
	UStartGameHUDBase* StartGameWidget;

	/* 本关开始前对话 */
	UPROPERTY(EditDefaultsOnly, Category = "Level Config")
	TArray<FSubtitleSetting> SubtitlesBeforeStartGame;

	TArray<FSubtitleSetting> ArrToShow;

	/* 本关倒计时 */
	UPROPERTY(EditDefaultsOnly, Category = "Level Config")
	float LevelTime{0.f};

	/* 每个房间位置，房间中心（第三人称位置），及宽高 */
	UPROPERTY(EditDefaultsOnly, Category = "Level Config")
	TArray<FRoomConfig> RoomInfo;

	/* 通关需要睡觉的秒数 */
	UPROPERTY(EditDefaultsOnly, Category = "Level Config")
	float WinSeconds{1.f};

	/* 本关睡觉特殊效果 */
	UPROPERTY(EditDefaultsOnly, Category = "Level Config")
	TArray<FDreamEffect> DreamEffects;

	/* 本关标题 */
	UPROPERTY(EditDefaultsOnly, Category = "Level Config")
	FString LevelName;

	/* 梦境进入时长，超过失败游戏 */
	UPROPERTY(EditDefaultsOnly, Category = "Level Config")
	float DreamMaxTime{1800.f};

	UPROPERTY(EditDefaultsOnly, Category = "Level Config")
	FString NextLevelName;

	/* 当前已经获取到的秒数 */
	UPROPERTY(BlueprintReadOnly)
	float CurrentSecondsCount{0.f};

	UPROPERTY(BlueprintReadOnly)
	/* 当前已经进入梦境的时长 */
	float CurrentDreamTime{0.f};

private:
	FTimerHandle SubtitleTimerHandle;

	UPROPERTY(Transient)
	AFirstPersonPlayerBase* CurrentPlayer;

	bool bInDream{false};

	float CurrentTime;
	bool bIsPause{false};
};
