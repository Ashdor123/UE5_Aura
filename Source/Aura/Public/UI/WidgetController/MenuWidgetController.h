// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "MenuWidgetController.generated.h"


class UAttributeInfo;
struct FAuraAttributeInfo;
//1.2 创建可以广播的委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

/**
 * 
 */

//1.蓝图类型意味着我们可以在事件图中把它当作一种类型使用，比如执行类型转换
//2.蓝图化意味着我们可以基于这个类创建蓝图
UCLASS(BlueprintType, Blueprintable)
class AURA_API UMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;

	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:
	//1.3主要用于通过游戏标签查找对应的属性信息
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;


private:

	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
