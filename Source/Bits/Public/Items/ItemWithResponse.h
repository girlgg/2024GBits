#pragma once

#include "CoreMinimal.h"
#include "InteractiveItemBase.h"
#include "Components/Items/DialogComponent.h"
#include "GameFramework/Actor.h"
#include "ItemWithResponse.generated.h"

class UDialogComponent;

UCLASS()
class BITS_API AItemWithResponse : public AInteractiveItemBase
{
	GENERATED_BODY()

public:
	AItemWithResponse();

	virtual void ExecInteractWith() override;
	void InputReply(int32 OptionAction);
	UFUNCTION(BlueprintCallable)
	void InputEndRelies();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_Option1();
	UFUNCTION(BlueprintImplementableEvent)
	void K2_Option2();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDialogComponent* DialogComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowResponse{true};

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FInteractionDialog> DialogSets;

public:
	virtual void Tick(float DeltaTime) override;
};
