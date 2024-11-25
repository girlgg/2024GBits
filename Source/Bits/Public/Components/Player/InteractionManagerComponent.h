#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionManagerComponent.generated.h"

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

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BITS_API UInteractionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionManagerComponent();

	bool CanInteract();
	virtual void RootInteraction();

	void CloseInspect();

protected:
	virtual void BeginPlay() override;

	void CheckInteraction();
	UFUNCTION(BlueprintCallable)
	virtual bool AllowInteraction();
	void InspectUI();
	void HasItemInteraction();

	AFirstPersonPlayerBase* GetPlayer();

	UPROPERTY(EditDefaultsOnly)
	float InteractionCheckTime{.1f};
	UPROPERTY(EditDefaultsOnly)
	float MaxRayDistance{300.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowInteraction{true};

	UPROPERTY(BlueprintReadOnly)
	AInteractiveItemBase* InteractiveItem;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInspectHUDBase> InspectHUDClass;

	UPROPERTY(BlueprintReadOnly, Transient)
	UInspectHUDBase* InspectHUD;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInputMappingContext* InspectMapping;

private:
	FTimerHandle InteractCheckTimer;

	UPROPERTY(Transient)
	AFirstPersonPlayerBase* CurrentPlayer{nullptr};

	EInteractionState CurrentInspectState;

public:
	bool bUseMouseLocation{false};
};
