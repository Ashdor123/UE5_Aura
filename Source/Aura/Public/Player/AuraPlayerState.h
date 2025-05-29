// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();

	//返回能力系统组件指针
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//返回属性集的指针
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
protected:
	//能力系统组件
	//Ps:玩家是在角色状态里创建的，敌人是在敌人类里创建的
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	//AttributeSet 属性集 Attribute属性
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
