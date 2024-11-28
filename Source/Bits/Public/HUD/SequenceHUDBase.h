#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SequenceHUDBase.generated.h"

class UTextBlock;
struct FSubtitleSetting;

UCLASS()
class BITS_API USequenceHUDBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void DisplaySubtitles(const TArray<FSubtitleSetting>& InSubtitles);
	void UpdateSubtitles();
	void UpdateText();
	void PlaySubtitle();

	void IntoDream();
	void OutDream();

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SubtitleText;

	/* 现实对话框背景 */
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* BackgroundT;
	/* 梦境对话框背景 */
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* BackgroundD;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* BackgroundImage;
	
	UPROPERTY(BlueprintReadOnly)
	FString TextToShow;

private:
	FTimerHandle TimerHandle;
	FTimerHandle TextTimerHandle;
	
	TArray<FSubtitleSetting> Subtitles;
	int32 CurrentSubtitleIndex{0};
	
	int32 CurrentTextIdx{0};
	FString CurrentText;

	float CurrentDelay{0};
	float CurrentTextDelay{0};
};
