#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	ExecFun,
	ExecFunWithItem,
	IntoDream,
	Max
};

UENUM(BlueprintType)
enum class EInspectMethod : uint8
{
	None,
	Readable,
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
	UTextBlock* InventoryIcon;

	/* 需要的背包里有的物品的名字 需要解锁的物品专用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InventoryItemName;
	/* 使用上面物品后是否被消耗 需要解锁的物品专用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ConsumeItem;

	/* 检视方法（可读） 可检视物品专用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInspectMethod InspectMethod;
	/* 可读的每一页的文字 可检视-可读 物品专用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> ReadableTextPages;

	bool operator==(const FInteractionMethods& Other) const
	{
		return InteractionMethod == Other.InteractionMethod &&
			PromptText == Other.PromptText &&
			InventoryIcon == Other.InventoryIcon &&
			InventoryItemName == Other.InventoryItemName &&
			ConsumeItem == Other.ConsumeItem &&
			InspectMethod == Other.InspectMethod &&
			ReadableTextPages == Other.ReadableTextPages;
	}
};

FORCEINLINE uint32 GetTypeHash(const FInteractionMethods& Methods)
{
	uint32 Hash = GetTypeHash(static_cast<uint8>(Methods.InteractionMethod));
	Hash = HashCombine(Hash, GetTypeHash(Methods.PromptText));
	Hash = HashCombine(Hash, PointerHash(Methods.InventoryIcon));
	Hash = HashCombine(Hash, GetTypeHash(Methods.InventoryItemName));
	Hash = HashCombine(Hash, GetTypeHash(Methods.ConsumeItem));
	Hash = HashCombine(Hash, GetTypeHash(static_cast<uint8>(Methods.InspectMethod)));
	for (const auto& Page : Methods.ReadableTextPages)
	{
		Hash = HashCombine(Hash, GetTypeHash(Page));
	}
	return Hash;
}

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
		return ObjectName == Other.ObjectName &&
			ObjectMesh == Other.ObjectMesh &&
			FMath::IsNearlyEqual(DisplayPromptFromRange, Other.DisplayPromptFromRange) &&
			InteractionPromptOffset == Other.InteractionPromptOffset &&
			InteractionMethod == Other.InteractionMethod;
	}
};

FORCEINLINE uint32 GetTypeHash(const FInteractiveData& Data)
{
	uint32 Hash = GetTypeHash(Data.ObjectName);
	Hash = HashCombine(Hash, PointerHash(Data.ObjectMesh));
	Hash = HashCombine(Hash, GetTypeHash(Data.DisplayPromptFromRange));
	Hash = HashCombine(Hash, GetTypeHash(Data.InteractionPromptOffset));
	Hash = HashCombine(Hash, GetTypeHash(Data.InteractionMethod));
	return Hash;
}

UCLASS()
class BITS_API AInteractiveItemBase : public AActor
{
	GENERATED_BODY()

public:
	AInteractiveItemBase();

	UFUNCTION(BlueprintCallable)
	virtual void PlayerHasItem();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInteractiveData InteractiveData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CustomCollisionRadius{32.f};

	virtual void Tick(float DeltaTime) override;

	void Outline(bool bEnable);

	UFUNCTION(BlueprintImplementableEvent)
	void InteractionExecFunc();

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
};
