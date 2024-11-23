#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveItemBase.generated.h"

class UInteractionWidgetComponent;
class USphereComponent;

UCLASS()
class BITS_API AInteractiveItemBase : public AActor
{
	GENERATED_BODY()

public:
	AInteractiveItemBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	/* 交互物体与交互键的碰撞体 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereCollision;
	/* 交互物体默认网格体 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	/* 交互可用时的UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInteractionWidgetComponent* InteractionWidgetComponent;
};
