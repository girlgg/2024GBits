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
	ChangeToFirstPerson();
}

void AFirstPersonPlayerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AFirstPersonPlayerBase::Move(const FInputActionValue& Value)
{
	if (!bIsPause && CurrentViewMode == EViewMode::FirstPerson)
	{
		FVector2D MovementVector = Value.Get<FVector2D>();

		if (Controller)
		{
			AddMovementInput(GetActorForwardVector(), MovementVector.Y);
			AddMovementInput(GetActorRightVector(), MovementVector.X);
		}
	}
}

void AFirstPersonPlayerBase::Look(const FInputActionValue& Value)
{
	if (!bIsPause && CurrentViewMode == EViewMode::FirstPerson)
	{
		FVector2D LookAxisVector = Value.Get<FVector2D>();

		if (Controller)
		{
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}

void AFirstPersonPlayerBase::Interact(const FInputActionValue& Value)
{
	if (!bIsPause)
	{
		InteractionManager->ClickInteraction();
	}
}

void AFirstPersonPlayerBase::Back(const FInputActionValue& Value)
{
	if (!bIsPause)
	{
		InteractionManager->CloseInspect();
		UGameplayFunctinos::UpdateInputMappingContext(GetWorld(), FirstPersonInputMapping);
	}
}

void AFirstPersonPlayerBase::Pause(const FInputActionValue& Value)
{
	if (bIsPause)
	{
		if (IsValid(PauseMenuHUD))
		{
			PauseMenuHUD->RemoveFromParent();
			PauseMenuHUD = nullptr;
		}
		BePause();
	}
	else
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
				}
			}
		}
		OutPause();
	}
}

void AFirstPersonPlayerBase::Navigate(const FInputActionValue& Value)
{
	FVector2D Val = Value.Get<FVector2D>();
	InventoryManager->Navigate(Val.X);
	InteractionManager->Navigate(Val.X);
}

void AFirstPersonPlayerBase::BePause()
{
	bIsPause = true;
	InteractionManager->bPause = true;
	if (ALevelGameModeBase* GM = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GM->PauseGame();
	}
}

void AFirstPersonPlayerBase::OutPause()
{
	bIsPause = false;
	InteractionManager->bPause = false;
	if (ALevelGameModeBase* GM = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GM->ContinueGame();
	}
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
	CameraManager->ShowCameraHUD();
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

	if (APlayerController* PC = GetController<APlayerController>())
	{
		PC->bShowMouseCursor = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
	}
	InteractionManager->bUseMouseLocation = true;
	UGameplayFunctinos::UpdateInputMappingContext(GetWorld(), ThirdPersonInputMapping);
	CameraManager->HideCameraHUD();
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->Velocity = FVector::ZeroVector;
		MovementComp->StopMovementImmediately();
	}
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
	if (GetController())
	{
		CurrentFirstRotator = GetController()->GetControlRotation();
	}

	if (ALevelGameModeBase* GM = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		const FRoomConfig& RoomInfo = GM->GetCurrentRoomPos(CurrentFirstPos);

		SetActorLocation(RoomInfo.ThirdPos);
		if (GetController())
		{
			GetController()->SetControlRotation(RoomInfo.ThirdRotation);
		}

		GM->IntoDream(InDreamTime);
	}

	ChangeViewMode(EViewMode::ThirdPerson);

	if (InventoryManager)
	{
		InventoryManager->IntoDream();
	}
	if (SequenceManager)
	{
		SequenceManager->IntoDream();
	}
}

void AFirstPersonPlayerBase::OutDream()
{
	SetActorLocation(CurrentFirstPos);
	if (GetController())
	{
		GetController()->SetControlRotation(CurrentFirstRotator);
	}

	if (ALevelGameModeBase* GM = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GM->OutDream();
	}

	ChangeViewMode(EViewMode::FirstPerson);

	if (InventoryManager)
	{
		InventoryManager->OutDream();
	}
	if (SequenceManager)
	{
		SequenceManager->OutDream();
	}
}
