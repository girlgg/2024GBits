#include "Components/Items/DialogComponent.h"

#include "Blueprint/UserWidget.h"
#include "Common/GameplayFunctinos.h"
#include "Components/Player/InteractionManagerComponent.h"
#include "Components/Player/SequenceManagerComponent.h"
#include "HUD/DialogResponse.h"
#include "Items/ItemWithResponse.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FirstPersonPlayerBase.h"


UDialogComponent::UDialogComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDialogComponent::InitializeConversation(AItemWithResponse* ResponseItem,
                                              const TMap<FString, FInteractionDialog>& InDialogSets)
{
	DialogSets = InDialogSets;
	for (const auto& Dialog : DialogSets)
	{
		CurrentDialogSet = Dialog.Value;
		break;
	}

	if (AFirstPersonPlayerBase* Player = UGameplayFunctinos::GetPlayer(GetWorld()))
	{
		Player->InteractionManager->InitiateConnersation(ResponseItem, false);
	}

	if (DialogMappingContext)
	{
		UGameplayFunctinos::UpdateInputMappingContext(GetWorld(), DialogMappingContext);
	}

	FString TargetReply = TEXT("{Initial}");
	if (!CurrentDialogSet.bRestartConversation)
	{
		TargetReply = CurrentDialogPlaying.Replies.SelectOnReply;
	}

	FDialog Dialog = FindDialogByTargetReply(TargetReply);

	UGameplayFunctinos::GetPlayer(this)->SequenceManager->PlaySubtitles(Dialog.Subtitles);

	if (AFirstPersonPlayerBase* Player =
		Cast<AFirstPersonPlayerBase>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		if (!Player->SequenceManager->OnSubtitlesFinished.IsBound())
		{
			Player->SequenceManager->OnSubtitlesFinished.BindUObject(this, &ThisClass::OnSubtitlesFinished);
		}
	}

	UpdateReplies(TargetReply);
	if (!CurrentDialogSet.bAllowPlayerMove)
	{
		// TODO 禁止动
	}
}

void UDialogComponent::InputReply(int32 ReplyOption)
{
	SelectedReplyOptionIndex = ReplyOption;
	FString ReplyOptionText = "{Initial}";
	int32 Condition = -1;
	if (bCanReply)
	{
		switch (SelectedReplyOptionIndex)
		{
		case 0:
			ReplyOptionText = CurrentDialogPlaying.Replies.ReplyOptionA;
			Condition = 0;
			break;
		case 1:
			ReplyOptionText = CurrentDialogPlaying.Replies.ReplyOptionB;
			Condition = 1;
			break;
		default:
			break;
		}
	}

	if (AFirstPersonPlayerBase* Player = UGameplayFunctinos::GetPlayer(GetWorld()))
	{
		FDialog ReplyDialog = FindDialogByTargetReply(ReplyOptionText);
		Player->SequenceManager->PlaySubtitles(ReplyDialog.Subtitles);
		Player->InteractionManager->OnDialogueReply.Broadcast(ReplyOptionText);
	}
	UpdateReplies(ReplyOptionText);

	if (AItemWithResponse* OwnItem = Cast<AItemWithResponse>(GetOwner()))
	{
		if (Condition == 0)
		{
			OwnItem->K2_Option1();
		}
		else if (Condition == 1)
		{
			OwnItem->K2_Option2();
		}
	}
}

void UDialogComponent::StopConversation()
{
	UGameplayFunctinos::RestoreMappingContext(GetWorld());
	CreateResponseWidget(true);

	UGameplayFunctinos::GetPlayer(GetWorld())->SequenceManager->OnSubtitlesFinished.Unbind();
}


void UDialogComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDialogComponent::UpdateReplies(const FString& InTargetReply)
{
	CreateResponseWidget(true);
	CurrentDialogPlaying = FindDialogByTargetReply(InTargetReply);
	bCanReply = false;
}

void UDialogComponent::CreateResponseWidget(bool bRemoved)
{
	if (bRemoved)
	{
		if (DialogResponseHUD)
		{
			DialogResponseHUD->RemoveFromParent();
			DialogResponseHUD = nullptr;
		}
	}
	else
	{
		if (UClass* LoadClass = DialogResponseHUDClass.LoadSynchronous())
		{
			if (!DialogResponseHUD)
			{
				DialogResponseHUD = CreateWidget<UDialogResponse>(GetWorld(), LoadClass);
				DialogResponseHUD->AddToViewport();
			}
		}
	}
}


void UDialogComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDialogComponent::OnSubtitlesFinished()
{
	if (!CurrentDialogPlaying.Replies.bIsLastReply)
	{
		CreateResponseWidget();
		if (DialogResponseHUD)
		{
			DialogResponseHUD->UpdateWidgetReplies(CurrentDialogPlaying.Replies.ReplyOptionA,
			                                       CurrentDialogPlaying.Replies.ReplyOptionB);
		}
	}
	else
	{
		if (AFirstPersonPlayerBase* Player = UGameplayFunctinos::GetPlayer(GetWorld()))
		{
			Player->InteractionManager->DialogInputReply(true);
			Player->InteractionManager->OnDialogueReply.Broadcast(TEXT("End"));
		}
	}
	bCanReply = true;
}

FDialog UDialogComponent::FindDialogByTargetReply(const FString& TargetReply)
{
	for (auto& Dialog : CurrentDialogSet.Dialogs)
	{
		if (Dialog.Replies.SelectOnReply == TargetReply)
		{
			return Dialog;
		}
	}
	for (auto& Dialog : CurrentDialogSet.Dialogs)
	{
		return Dialog;
	}
	return FDialog();
}
