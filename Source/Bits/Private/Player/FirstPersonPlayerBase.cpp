#include "Player/FirstPersonPlayerBase.h"

#include "EnhancedInputComponent.h"
#include "Common/GameplayFunctinos.h"
#include "Components/Player/CameraManagerComponent.h"
#include "Components/Player/InteractionManagerComponent.h"
#include "Components/Player/InventoryManagerComponent.h"

AFirstPersonPlayerBase::AFirstPersonPlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraManager = CreateDefaultSubobject<UCameraManagerComponent>(TEXT("CameraManager"));
	InteractionManager = CreateDefaultSubobject<UInteractionManagerComponent>(TEXT("InteractionManager"));
	InventoryManager = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("InventoryManager"));
}

void AFirstPersonPlayerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AFirstPersonPlayerBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFirstPersonPlayerBase::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFirstPersonPlayerBase::Interact(const FInputActionValue& Value)
{
	InteractionManager->RootInteraction();
}

void AFirstPersonPlayerBase::Back(const FInputActionValue& Value)
{
	InteractionManager->CloseInspect();
	UGameplayFunctinos::UpdateInputMappingContext(GetWorld(), FirstPersonInputMapping);
}

void AFirstPersonPlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFirstPersonPlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);
		EnhancedInputComponent->BindAction(BackAction, ETriggerEvent::Started, this, &ThisClass::Back);
	}
}
