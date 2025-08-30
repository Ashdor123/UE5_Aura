// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"


class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;
class UGameplayEffect;

//效果应用策略
UENUM(BlueprintType)
enum class EEffectApplicationPolicy :uint8
{
	ApplyOnOverlay,
	ApplyOnEndOverlay,
	DoNotApply
};

//效果移除策略
//移除策略主要针对无限时长的游戏效果，因为有持续时间的效果会在时间结束时自动消失
UENUM(BlueprintType)
enum class EEffectRemovalPolicy :uint8
{
	RemoveOnEndOverlay,
	DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AAuraEffectActor();

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);
	
	UFUNCTION(BlueprintCallable)
	void OnOverlay(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlay(AActor* TargetActor);
	
	//是否删除效果
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	bool bDestroyOnEffectRemoval = false;
	
	//瞬时效果类
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	//瞬时时间效果策略
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	//持续时间效果类
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	//持续时间效果策略
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	//无限时间效果类
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	//无限时间效果策略
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	//无限时间移除策略
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy;

	
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	//效果等级
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effect")
	float ActorLevel = 1.f;
};
