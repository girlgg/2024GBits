#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraManagerComponent.generated.h"


class ACameraManagerBase;
enum class EViewMode : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BITS_API UCameraManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCameraManagerComponent();
	void StartGame();

	void ShowCameraHUD();
	void HideCameraHUD();

	void CreateCameraHUD();
	void DestroyCameraHUD();

protected:
	virtual void BeginPlay() override;

	ACameraManagerBase* GetCameraManager();

	UPROPERTY(Transient)
	ACameraManagerBase* CameraManager{nullptr};
};
