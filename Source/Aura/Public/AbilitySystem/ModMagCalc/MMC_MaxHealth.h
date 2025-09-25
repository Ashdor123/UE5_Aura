// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	//1.创建构造函数用于初始化
	UMMC_MaxHealth();

	//2.用于计算自定义数值
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	//3.定义我们要捕捉的属性
	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
