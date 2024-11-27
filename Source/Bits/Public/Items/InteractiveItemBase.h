#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Common/SubtitleSetting.h"
#include "InteractiveItemBase.generated.h"

class UTextBlock;
class UInteractionWidgetComponent;
class USphereComponent;

UENUM(BlueprintType)
enum class EInteractionMethod : uint8
{
	None,
	Pickup,
	Inspect,
	HasItem,
	IntoDream,
	OutDream,
	Max
};

UENUM(BlueprintType)
enum class EInspectMethod : uint8
{
	None,
	Readable,
	Say,
	Max
};

USTRUCT(BlueprintType)
struct FInteractionMethods
{
	GENERATED_BODY()

	/* 交互方法，只能捡，检视，需要物品解锁 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteractionMethod InteractionMethod;
	/* 靠近提示文字 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PromptText;

	/* 背包里的图标 被拾取的物品专用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* InventoryIcon;
	/* 拾取后是否显示在背包，可拾取物品专用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowInInventory{true};
	/* 允许拾取的最大数量，可拾取物品专用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxItemNum{99};

	/* 需要的背包里有的物品的名字 需要解锁的物品专用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InventoryItemName;
	/* 使用上面物品后是否被消耗 需要解锁的物品专用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ConsumeItem;
	/*  必须在背包选择该物品才能使用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool MustSelectedFirst{false};

	/* 检视方法（可读） 可检视物品专用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInspectMethod InspectMethod;
	/* 可读的每一页的文字 可检视-可读 物品专用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> ReadableTextPages;
	/* 说话的每一句的文字 可检视-说话 物品专用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSubtitleSetting> SayTextPages;

	/* 本次进入睡眠的时间，进入睡眠物体专用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DreamGetTime;
};

USTRUCT(BlueprintType)
struct FInteractiveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ObjectMesh;
	/* 多少半径内显示提示 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DisplayPromptFromRange;
	/* 提示显示的位置偏移 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector InteractionPromptOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInteractionMethods InteractionMethod;

	bool operator==(const FInteractiveData& Other) const
	{
		return ObjectName == Other.ObjectName;
	}
};

FORCEINLINE uint32 GetTypeHash(const FInteractiveData& Data)
{
	uint32 Hash = GetTypeHash(Data.ObjectName);
	return Hash;
}

UCLASS()
class BITS_API AInteractiveItemBase : public AActor
{
	GENERATED_BODY()

public:
	AInteractiveItemBase();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerHasItem();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnIntoDream();
	UFUNCTION()
	void OnOutDream();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_IntoDream();
	UFUNCTION(BlueprintImplementableEvent)
	void K2_OutDream();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInteractiveData InteractiveData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CustomCollisionRadius{32.f};

	virtual void Tick(float DeltaTime) override;

	void Outline(bool bEnable);

protected:
	UFUNCTION()
	void OnPlayerInUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                  const FHitResult& SweepResult);
	UFUNCTION()
	void OnPlayerOutUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/* 交互物体与交互键的碰撞体 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereCollision;
	/* 交互物体与玩家的碰撞体，碰撞则显示提示UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* UICollision;
	/* 交互物体默认网格体 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	/* 交互可用时的UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInteractionWidgetComponent* InteractionWidgetComponent;
	/* 进入梦境后的网格 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> IntoDreamMesh;
	/* 离开梦境后的网格 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> OutDreamMesh;
};
