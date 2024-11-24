#include "Components/Player/InteractionManagerComponent.h"

#include "Items/InteractiveItemBase.h"


UInteractionManagerComponent::UInteractionManagerComponent()
{
}

bool UInteractionManagerComponent::CanInteract()
{
	return InteractiveItem != nullptr;
}

void UInteractionManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(InteractCheckTimer,
	                                       this,
	                                       &ThisClass::CheckInteraction, InteractionCheckTime,
	                                       false);
}

void UInteractionManagerComponent::CheckInteraction()
{
	if (InteractiveItem)
	{
		InteractiveItem->Outline(false);
		InteractiveItem = nullptr;
	}

	if (AllowInteraction())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			FVector CameraLocation;
			FRotator CameraRotation;
			PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

			FVector Start = CameraLocation;
			FVector End = Start + (CameraRotation.Vector() * MaxRayDistance);

			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(GetOwner());

			if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, Params))
			{
				AActor* HitActor = HitResult.GetActor();
				if (HitActor)
				{
					InteractiveItem = Cast<AInteractiveItemBase>(HitActor);
					if (InteractiveItem)
					{
						InteractiveItem->Outline(true);
					}
				}
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(InteractCheckTimer,
	                                       this,
	                                       &ThisClass::CheckInteraction, InteractionCheckTime,
	                                       false);
}

bool UInteractionManagerComponent::AllowInteraction()
{
	return bAllowInteraction;
}
