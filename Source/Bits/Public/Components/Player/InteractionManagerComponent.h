#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/InteractiveItemBase.h"
#include "InteractionManagerComponent.generated.h"

class AItemWithResponse;
class UInputMappingContext;
class UInspectHUDBase;
class AFirstPersonPlayerBase;
class AInteractiveItemBase;

UENUM()
enum class EInteractionState : uint8
{
	None,
	Inspecting,
	Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteracted, FInteractiveData, TargetInteractionActorData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueReply, FString, ReplyOption);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BITS_API UInteractionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionManagerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool CanInteract();
	void ClickInteraction();
	virtual void RootInteraction();

	void CloseInspect();

	void Navigate(float Direction);

	void DialogInputReply(bool bInEnd = false, int32 Option = 0);
	void InitiateConnersation(AItemWithResponse* ItemWithResponse, bool bStopCondition);

	FOnInteracted OnInteracted;
	FOnDialogueReply OnDialogueReply;

	UPROPERTY(Transient)
	AItemWithResponse* ResponseItem;

protected:
	virtual void BeginPlay() override;

	void CheckInteraction(float DeltaTime);

	void InspectUI();
	bool HasItemInteraction();

	void ReadPaper();
	void MakeSay();

	void DoorInteraction();

	UFUNCTION(BlueprintCallable)
	void IntoDream(float InDreamTime);
	void OutDream();

	void InspectFromData();

	AFirstPersonPlayerBase* GetPlayer();

	UPROPERTY(EditDefaultsOnly)
	float MaxRayDistance{300.f};

	UPROPERTY(BlueprintReadOnly)
	AInteractiveItemBase* InteractiveItem;

	UPROPERTY(Transient)
	AInteractiveItemBase* LastInteractiveItem;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInspectHUDBase> InspectHUDClass;

	UPROPERTY(BlueprintReadOnly, Transient)
	UInspectHUDBase* InspectHUD;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInputMappingContext* InspectMapping;

private:
	UPROPERTY(Transient)
	AFirstPersonPlayerBase* CurrentPlayer{nullptr};

	EInteractionState CurrentInspectState;

	float CurrentGazeTime{0.f};

	bool bIsGazeInteraction{false};

public:
	bool bUseMouseLocation{false};
	bool bPause{false};
	bool bAllowInteraction{false};
};
