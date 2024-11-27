// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameOverGameMode.generated.h"

class UGameOverHUDBase;

UCLASS()
class BITS_API AGameOverGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UGameOverHUDBase> GameOverUIClass;

	UPROPERTY(Transient)
	UGameOverHUDBase* GameOverUI;
};
