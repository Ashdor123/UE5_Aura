// Ashdor


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


AAuraEffectActor::AAuraEffectActor()
{
 	
	PrimaryActorTick.bCanEverTick =false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	//得到能力系统接口
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	//检查
	if (TargetASC == nullptr) return;
	check(GameplayEffectClass);
	
	//MakeEffectContext()就是用来创建包含内部游戏效果上下文的包装器,我们可以在那个上下文里设置内容
	//游戏效果上下文可以存储与游戏效果相关的各种信息
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	
	//1.添加源对象
	EffectContextHandle.AddSourceObject(this);
	
	//设置规格权柄
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	//Prediction预测指的是延迟补偿中的预测机制
	//EffectSpecHandle.Data.Get()获取原始指针后解引用
	FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	//要检查持续时间策略，我们得看看效果规格句柄,如果它是无限的，那么这个表达式就会返回true
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlay)
	{
		//把FActiveGameplayEffectHandle ActiveEffectHandle（活动效果句柄）和 TargetASC (目标能力系统组件)添加到我的活动效果句柄映射中
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
		
	}
}

void AAuraEffectActor::OnOverlay(AActor* TargetActor)
{
	//瞬时
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlay)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	//持续时间
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlay)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	//无限时间
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlay)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlay(AActor* TargetActor)
{
	//瞬时
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlay)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	//持续时间
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlay)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	//无限时间
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlay)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	//无限时间中如何移除效果
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlay)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		//创建一个用于在循环中记录需要移除效果的数组
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TPair<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			//如果相等，我们就移除这个游戏效果
			if (TargetASC == HandlePair.Value)
			{
				//移除一层堆叠，而不是删除所有的堆叠
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			//我们要用键从Map里把那个特定元素删掉，这样就把激活的游戏效果从对应的能力系统组件中移除了
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

