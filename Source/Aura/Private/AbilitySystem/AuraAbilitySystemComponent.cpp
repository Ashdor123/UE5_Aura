// Ashdor


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilites/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//绑定(这行函数只在服务器上运行 -- RPC) 这个委托一旦在服务器上广播，就会在服务器端被调用，并在客户端执行
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuraAbilitySystemComponent::ClientEffectApplied);

	// const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	// GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Blue,FString::Printf(TEXT("Tags ：%s"),*GameplayTags.Attributes_Secondary_Armor.ToString()));
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		//1.创建游戏规格
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability) )
		{
			//向启动能力的动态能力标签里添加一个输入标签（DynamicAbilityTags 是在能力实例被给予时可以动态添加的标签，它们可以用于激活、取消或查询能力。）
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartInputTag);
			//将前面创建并配置好的 FGameplayAbilitySpec 实例给予给拥有这个 Ability System Component 的角色
			GiveAbility(AbilitySpec);
		}
	}
}

//在玩家控制器中调用
void UAuraAbilitySystemComponent::AbilityInputHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	//遍历所有可激活的能力
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		
		//如果这个能力和输入标签完全对应
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				//激活·标签
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	//遍历所有可激活的能力
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//如果这个能力和输入标签完全对应
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
			
		}
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	//1.创建标签容器
	FGameplayTagContainer TagContainer;
	//2.通过效果规格获取所有资产标签（对容器里的每个标签都向我们的Widget控制器广播一次,从而我们就能获取这些标签，广播出去，用来在屏幕上显示一些消息）
	EffectSpec.GetAllAssetTags(TagContainer);
	
	EffectAssetTags.Broadcast(TagContainer);
}
