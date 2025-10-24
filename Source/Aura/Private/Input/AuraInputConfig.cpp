// Ashdor


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FAuraInputAction& InputAction : AbilityInputActions)
	{
		//检查数组中对应的值与传入的参数标签值是否相对应
		if (InputAction.InputTag == InputTag && InputAction.InputAction)
		{
			return InputAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("UInputAction 在 AuraInputConfig类 中没有找到!"));
	}
	return nullptr;
}
