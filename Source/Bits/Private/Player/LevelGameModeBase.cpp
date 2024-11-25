#include "Player/LevelGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "HUD/CountdownHUDBase.h"

void ALevelGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (CountdownWidgetClass)
	{
		CountdownWidget = CreateWidget<UCountdownHUDBase>(GetWorld(), CountdownWidgetClass);
		if (CountdownWidget)
		{
			CountdownWidget->AddToViewport();
		}
	}
}
