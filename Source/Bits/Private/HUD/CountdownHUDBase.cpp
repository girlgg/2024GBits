#include "HUD/CountdownHUDBase.h"

#include "Components/ProgressBar.h"

void UCountdownHUDBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (ProgressBar)
	{
		ProgressBar->SetPercent(1.f);
	}
}

void UCountdownHUDBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CurrentTime > 0)
	{
		CurrentTime -= InDeltaTime;
		float Progress = FMath::Clamp(CurrentTime / MaxTime, 0.f, 1.f);
		if (IsValid(ProgressBar))
		{
			ProgressBar->SetPercent(Progress);
		}
	}
}

void UCountdownHUDBase::SetTime(float InTime)
{
	MaxTime = InTime;
	CurrentTime = MaxTime;
}
