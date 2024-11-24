#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FirstPersonPlayerBase.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraManagerComponent* CameraManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInteractionManagerComponent* InteractionManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventoryManagerComponent* InventoryManager;

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	UInputAction* MoveAction;
};
