// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
/**
 *
 * 这是一个包含原生游戏标签的单例，这里的原生指的是这些标签由C++内部创建，并且可以在C++中使用，同样也可以在编辑器与蓝图中使用
 */
struct FAuraGameplayTags
{
public:
	//1.这个静态函数会返回游戏中唯一存在的FAuragameplaytags实例的引用
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	//4.
	static void InitializeNativeGameplayTags();
	/*Primary Attributes*/
	FGameplayTag Attribute_Primary_Strength;

	FGameplayTag Attribute_Primary_Intelligence;

	FGameplayTag Attribute_Primary_Resilience;

	FGameplayTag Attribute_Primary_Vigor;
	/*Primary Attributes End*/
	
	/** Secondary Attributes */
	//5.创建Armor的变量
	FGameplayTag Attributes_Secondary_Armor;

	FGameplayTag Attributes_Secondary_ArmorPenetration;

	FGameplayTag Attributes_Secondary_BlockChance;
	
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	
	FGameplayTag Attributes_Secondary_CriticalResistance;
	
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	
	FGameplayTag Attributes_Secondary_MaxHealth;
	
	FGameplayTag Attributes_Secondary_MaxMana;
	/** Secondary Attributes End*/
protected:
	

private:
	//2.静态变量
	static FAuraGameplayTags GameplayTags;
};
