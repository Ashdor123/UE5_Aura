// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	//用于在最开始赋予技能的时候进行检查
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartInputTag;

	
};
