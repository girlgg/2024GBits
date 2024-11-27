#include "Player/FirstPersonPlayerBase.h"

#include "EnhancedInputComponent.h"
#include "Common/GameplayFunctinos.h"
#include "Components/Player/CameraManagerComponent.h"
#include "Components/Player/InteractionManagerComponent.h"
#include "Components/Player/InventoryManagerComponent.h"
#include "Components/Player/SequenceManagerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/PauseMenuHUDBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/LevelGameModeBase.h"

AFirstPersonPlayerBase::AFirstPersonPlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraManager = CreateDefaultSubobject<UCameraManagerComponent>(TEXT("CameraManager"));
	InteractionManager = CreateDefaultSubobject<UInteractionManagerComponent>(TEXT("InteractionManager"));
	InventoryManager = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("InventoryManager"));
	SequenceManager = CreateDefaultSubobject<USequenceManagerComponent>(TEXT("SequenceManager"));
}

void AFirstPersonPlayerBase::StartGame()
{
	UGameplayFunctinos::UpdateInputMappingContext(GetWorld(), FirstPersonInputMapping);
	CameraManager->StartGame();
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

void AFirstPersonPlayerBase::Pause(const FInputActionValue& Value)
{
	if (IsValid(PauseMenuHUD))
	{
		PauseMenuHUD->RemoveFromParent();
		PauseMenuHUD = nullptr;
	}
	else if (IsValid(PauseMenuHUDClass))
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			PauseMenuHUD = CreateWidget<UPauseMenuHUDBase>(PlayerController, PauseMenuHUDClass);
			if (PauseMenuHUD)
			{
				PauseMenuHUD->AddToViewport();

				FInputModeUIOnly InputMode;
				PlayerController->SetInputMode(InputMode);
				PlayerController->bShowMouseCursor = true;
			}
		}
	}
}

void AFirstPersonPlayerBase::Navigate(const FInputActionValue& Value)
{
	FVector2D Val = Value.Get<FVector2D>();
	InventoryManager->Navigate(Val.X);
}

void AFirstPersonPlayerBase::ChangeToFirstPerson()
{
	if (CurrentViewMode != EViewMode::FirstPerson)
	{
		return;
	}
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->GravityScale = 1.f;
	}

	if (APlayerController* PC = GetController<APlayerController>())
	{
		PC->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
	InteractionManager->bUseMouseLocation = false;
	UGameplayFunctinos::UpdateInputMappingContext(GetWorld(), FirstPersonInputMapping);
	CameraManager->CreateCameraHUD();
}

void AFirstPersonPlayerBase::ChangeToThirdPerson()
{
	if (CurrentViewMode != EViewMode::ThirdPerson)
	{
		return;
	}
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->GravityScale = 0.f;
	}
	SetActorLocation(FVector(0, 0, 800));
	if (GetController())
	{
		GetController()->SetControlRotation(FRotator(-90, 0, 0));
	}

	if (APlayerController* PC = GetController<APlayerController>())
	{
		PC->bShowMouseCursor = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
	}
	InteractionManager->bUseMouseLocation = true;
	UGameplayFunctinos::UpdateInputMappingContext(GetWorld(), ThirdPersonInputMapping);
	CameraManager->DestroyCameraHUD();
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
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ThisClass::Pause);
		EnhancedInputComponent->BindAction(NavigateAction, ETriggerEvent::Started, this, &ThisClass::Navigate);
	}
}

void AFirstPersonPlayerBase::ChangeViewMode(EViewMode NewViewMode)
{
	if (CurrentViewMode == NewViewMode)
	{
		return;
	}
	CurrentViewMode = NewViewMode;
	switch (NewViewMode)
	{
	case EViewMode::FirstPerson:
		ChangeToFirstPerson();
		break;
	case EViewMode::ThirdPerson:
		ChangeToThirdPerson();
		break;
	case EViewMode::Max:
		break;
	}
}

void AFirstPersonPlayerBase::IntoDream(float InDreamTime)
{
	CurrentFirstPos = GetActorLocation();

	if (ALevelGameModeBase* GM = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		SetActorLocation(GM->GetCurrentRoomPos(CurrentFirstPos));
		GM->IntoDream(InDreamTime);
	}

	ChangeViewMode(EViewMode::ThirdPerson);
}

void AFirstPersonPlayerBase::OutDream()
{
	SetActorLocation(CurrentFirstPos);

	if (ALevelGameModeBase* GM = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GM->OutDream();
	}

	ChangeViewMode(EViewMode::ThirdPerson);
}
