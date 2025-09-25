// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"


//属性访问宏
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//根据Unreal的编码规范，typedef的名字应该以它所代表的类型的前缀开头。注意：typedef是专门针对F游戏玩法属性的签名，但TStaticFunkPointer是通用的，适用于任意签名
//typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;

//模版别名
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;



/**
 * 
 */
USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY();

	FEffectProperties() {}
	
	UPROPERTY()
	FGameplayEffectContextHandle EffectContextHandle;

	//来源
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> SourceAvatarActor = nullptr;

	UPROPERTY()
	TObjectPtr<AController> SourceController = nullptr;

	UPROPERTY()
	TObjectPtr<ACharacter> SourceCharacter = nullptr;

	//目标
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> TargetAvatarActor = nullptr;

	UPROPERTY()
	TObjectPtr<AController> TargetController = nullptr;

	UPROPERTY()
	TObjectPtr<ACharacter> TargetCharacter = nullptr;
};

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	//一般会在构造函数里初始化所有属性，确保角色创建时有正确的初始值
	UAuraAttributeSet();

	//实现数据限制的方法函数（第一种）
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	//后置游戏效果执行功能
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	//用于联系属性标签和属性的Map  ----TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	
/*----------------------------------------这是设置属性同步的基本步骤---------------------------------*/
	//1.设置健康属性
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Health, Category = "vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Health);//6.添加属性访问器
	
	//2.设定参数，意义在于当健康值被复制的时候，它会等于新值，但我们仍能知道旧值，因为旧值被传递了进来
	//Ps：这是rep通知的一个特性,要记得更新通知还要是UFUNCTION()
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
	//4.用于同步任意变量的函数
	//OutLifetimeProps 输出生命周期属性,GetLifetimeReplicatedProps 获取生命周期复制属性 Props道具
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//-----------------------------------------------------------------------------------------------------

	/*
	 * 主要属性
	 */
	//力量
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Strength);

	//智力
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Intelligence);

	//韧性
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Resilience);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Vigor);
	
	/*
	 * 主要属性的更新通知
	 */
	
	//设置力量属性的更新通知
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;
	
	/*
	 * 次要属性
	 */
	
	//护甲
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Armor);
	//护甲穿透
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,ArmorPenetration);
	//格挡机率
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,BlockChance);
	//暴击率
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CriticalHitChance);
	//暴击伤害
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CriticalHitDamage);
	//暴击抵抗
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_CriticalResistance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CriticalResistance);
	//生命值回复速度
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,HealthRegeneration);
	//魔法回复速度
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,ManaRegeneration);
	//设置最大魔力属性
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxMana, Category = "vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxMana);
	//设置最多健康属性
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxHealth, Category = "vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxHealth);
	
	/*
	* 次要属性的更新通知
	*/
	
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	UFUNCTION()
	void OnRep_CriticalResistance(const FGameplayAttributeData& OldCriticalResistance) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;
	
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	//设置最大生命值的更新通知
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	//设置最大魔力的更新通知
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	
	/*
	 * 重要属性
	 */
	
	//设置现有魔力属性
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Mana, Category = "vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Mana);
	
	//设置现有魔力属性的更新通知
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	
	
	//---------------------------------------------------
	

	

private:
	
	void SetEffectProperties(const struct FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};
