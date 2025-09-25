// Ashdor


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	//4.指定要捕捉的属性(从UAuraAttributeSet中获取,访问属性集中的属性）;
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute(); //GetVigorAttribute()是静态函数，这意味着我们不必持有对象或属性集对象的指针,我们可以直接调用这个静态函数来获取属性值
	//5.设置属性的来源;
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	//6.设置是否需要快照;
	//PS:快照捕捉就是在效果规格一创建，就马上捕捉属性,不快照的话就是在效果应用时才去获取真正的属性值
	VigorDef.bSnapshot = false;
	//7.创建需要捕获的相关属性的数组，将属性添加进数组
	RelevantAttributesToCapture.Add(VigorDef); 
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//如何捕获员和目标的标签
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	//8.创建评估参数并设置评估参数的源和目标标签
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	//9.得到实际的活力值
	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	//让Vigor的值大于零
	Vigor = FMath::Max<float>(Vigor, 0.0f);
	
	//10.获取源对象,通过接口的方式获取源对象的等级
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	
	//11.返回值
	return 80.f + 2.5 * Vigor + 10.f * PlayerLevel;
	
}
