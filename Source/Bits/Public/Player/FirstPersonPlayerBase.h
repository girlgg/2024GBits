#pragma once

#include "CoreMinimal.h"
#include "Common/ViewMode.h"
#include "GameFramework/Character.h"
#include "FirstPersonPlayerBase.generated.h"

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

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void ChangeViewMode(EViewMode NewViewMode);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraManagerComponent* CameraManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInteractionManagerComponent* InteractionManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventoryManagerComponent* InventoryManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USequenceManagerComponent* SequenceManager;

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Back(const FInputActionValue& Value);
	void Pause(const FInputActionValue& Value);

	void ChangeToFirstPerson();
	void ChangeToThirdPerson();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* BackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* PauseAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* FirstPersonInputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* ThirdPersonInputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UPauseMenuHUDBase> PauseMenuHUDClass;
	UPROPERTY(BlueprintReadOnly, Category = "HUD", Transient)
	UPauseMenuHUDBase* PauseMenuHUD;

	UPROPERTY(BlueprintReadWrite)
	EViewMode CurrentViewMode{EViewMode::FirstPerson};
};
