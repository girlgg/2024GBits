#include "Player/GameOverGameMode.h"

#include "HUD/GameOverHUDBase.h"

void AGameOverGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GameOverUIClass))
	{
		if (GameOverUIClass)
		{
			GameOverUI = CreateWidget<UGameOverHUDBase>(GetWorld(), GameOverUIClass);
			if (GameOverUI)
			{
				GameOverUI->AddToViewport();
			}
		}
	}
}
