#pragma once

#include "CoreMinimal.h"
#include "Common/SubtitleSetting.h"
#include "Components/ActorComponent.h"
#include "DialogComponent.generated.h"

class AItemWithResponse;
class UDialogResponse;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FReplies
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SelectOnReply;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ReplyOptionA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ReplyOptionB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLastReply;
};

USTRUCT(BlueprintType)
struct FDialog
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FReplies Replies;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSubtitleSetting> Subtitles;
};

USTRUCT(BlueprintType)
struct FInteractionDialog
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRestartConversation{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowPlayerMove{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDialog> Dialogs;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BITS_API UDialogComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDialogComponent();

	void InitializeConversation(AItemWithResponse* ResponseItem, const TMap<FString, FInteractionDialog>& InDialogSets);
	void InputReply(int32 ReplyOption);
	void StopConversation();

protected:
	virtual void BeginPlay() override;

	/* 按回复选项更新对话 */
	void UpdateReplies(const FString& InTargetReply);
	/* 创建回复UI */
	void CreateResponseWidget(bool bRemoved = false);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
	void OnSubtitlesFinished();

	FDialog FindDialogByTargetReply(const FString& TargetReply);

protected:
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* DialogMappingContext;
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UDialogResponse> DialogResponseHUDClass;
	UPROPERTY(Transient)
	UDialogResponse* DialogResponseHUD;

private:
	FInteractionDialog CurrentDialogSet;
	FDialog CurrentDialogPlaying;
	TMap<FString, FInteractionDialog> DialogSets;
	int32 SelectedReplyOptionIndex{0};
	bool bCanReply{false};
};
