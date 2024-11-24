// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionManagerComponent.generated.h"


class AInteractiveItemBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BITS_API UInteractionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionManagerComponent();

	bool CanInteract();

protected:
	virtual void BeginPlay() override;

	void CheckInteraction();
	UFUNCTION(BlueprintCallable)
	virtual bool AllowInteraction();

	UPROPERTY(EditDefaultsOnly)
	float InteractionCheckTime{.1f};
	UPROPERTY(EditDefaultsOnly)
	float MaxRayDistance{300.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowInteraction{true};

	UPROPERTY(BlueprintReadOnly)
	AInteractiveItemBase* InteractiveItem;

private:
	FTimerHandle InteractCheckTimer;
};
