// Ashdor


#include "UI/WidgetController/MenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UMenuWidgetController::BroadcastInitialValues()
{
	
	//1.获取属性集
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	// //1.4查找标签和设置属性与绑定
	// //实际上我们是从Aura游戏标签中提取力量属性的标签,然后从属性信息数据资源中找到对应的属性信息结构体
	// FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attribute_Primary_Strength);
	// //接着，我们从属性集中取出力量的数值，设置到属性信息里
	// Info.AttributeValue = AS->GetStrength();
	// //把这些信息打包成结构体后，我们广播给组件，组件就能接收到
	// AttributeInfoDelegate.Broadcast(Info);

	//1.广播初始值 在控件控制器里，我们遍历这张映射表，找到对应游戏标签的属性信息，标签作为键
	for (auto& pair : AS->TagsToAttributes )
	{
		BroadcastAttributeInfo(pair.Key, pair.Value());
	}
}

void UMenuWidgetController::BindCallbacksToDependencies() 
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	for (auto& pair : AS->TagsToAttributes )
	{
		//现在我们得到了map中每个属性对应的游戏玩法属性值变化委托
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(pair.Value()).AddLambda(
		[this, pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(pair.Key, pair.Value());
		}

		);

	}

}

void UMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	//实际上我们是从Aura游戏标签中提取属性的标签,然后从属性信息数据资源中找到对应的属性信息结构体
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	//用函数指针调用函数时，只要像调用普通函数一样加上括号就行了
	const FGameplayAttribute Attr = Attribute;
	//Fgameplay属性本身是通过静态函数返回的,我们必须知道该属性是属于哪个属性集
	Info.AttributeValue = Attr.GetNumericValue(AttributeSet);
	//把这些信息打包成结构体后，我们广播给控件，控件就能接收到并显示
	AttributeInfoDelegate.Broadcast(Info);
}
