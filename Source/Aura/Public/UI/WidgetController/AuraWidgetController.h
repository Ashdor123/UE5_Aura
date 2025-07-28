// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

protected:

	//下面的4个变量将用于获取数据，并向我们的控件广播
	//玩家控制器
	UPROPERTY(BlueprintReadWrite, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	//玩家状态
	UPROPERTY(BlueprintReadWrite, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	//能力系统组件
	UPROPERTY(BlueprintReadWrite, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	//属性集
	UPROPERTY(BlueprintReadWrite, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
