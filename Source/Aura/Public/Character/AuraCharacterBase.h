// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	AAuraCharacterBase();
	//返回能力系统组件指针
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//返回属性集的指针
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	//能力系统组件
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	//AttributeSet 属性集 Attribute属性
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
