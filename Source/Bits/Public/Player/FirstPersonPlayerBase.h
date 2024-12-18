#pragma once

#include "CoreMinimal.h"
#include "Common/ViewMode.h"
#include "GameFramework/Character.h"
#include "FirstPersonPlayerBase.generated.h"

class USphereComponent;
class UCISCharacterInteractionComponent;
struct FInteractiveData;
class UPauseMenuHUDBase;
class USequenceManagerComponent;
class UInputMappingContext;
class UCameraManagerComponent;
class UInventoryManagerComponent;
class UInteractionManagerComponent;
struct FInputActionValue;
class UInputAction;

UCLASS()
class BITS_API AFirstPersonPlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	AFirstPersonPlayerBase();

	void StartGame();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void ChangeViewMode(EViewMode NewViewMode);

	void IntoDream(float InDreamTime);
	void OutDream();

	void BePause();
	void OutPause();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCameraManagerComponent> CameraManager;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInteractionManagerComponent> InteractionManager;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInventoryManagerComponent> InventoryManager;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USequenceManagerComponent> SequenceManager;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USphereComponent> FirstTriggerCollision;

	UPROPERTY(BlueprintReadWrite)
	EViewMode CurrentViewMode{EViewMode::FirstPerson};

	UFUNCTION()
	void OnInteractedInput(FInteractiveData TargetInteractionActorData);

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void SubInteract(const FInputActionValue& Value);
	void Back(const FInputActionValue& Value);
	void Pause(const FInputActionValue& Value);
	void Navigate(const FInputActionValue& Value);

	void ChangeToFirstPerson();
	void ChangeToThirdPerson();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* SubInteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* BackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* PauseAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* NavigateAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* FirstPersonInputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* ThirdPersonInputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UPauseMenuHUDBase> PauseMenuHUDClass;
	UPROPERTY(BlueprintReadOnly, Category = "HUD", Transient)
	UPauseMenuHUDBase* PauseMenuHUD;

private:
	FVector CurrentFirstPos;
	FRotator CurrentFirstRotator;

	bool bIsPause{false};
};
