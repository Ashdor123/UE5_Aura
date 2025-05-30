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
/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	//一般会在构造函数里初始化所有属性，确保角色创建时有正确的初始值
	UAuraAttributeSet();
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

	
	//设置最多健康属性
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxHealth, Category = "vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxHealth);
	
	//设置现有魔力属性
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Mana, Category = "vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Mana);
	
	//设置最大魔力属性
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxMana, Category = "vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxMana);
	
	//设置最大生命值的更新通知
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	
	//设置现有魔力属性的更新通知
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	
	//设置最大魔力的更新通知
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
};
