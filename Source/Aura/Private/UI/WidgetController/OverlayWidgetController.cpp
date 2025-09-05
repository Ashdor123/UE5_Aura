// Ashdor


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

//调用时，必须确保属性集是有效的，在所有控件设置完成后
void UOverlayWidgetController::BroadcastInitialValues()
{
	//将数值广播给界面上的控件，可以通过创建委托的方式来广播数值
	const UAuraAttributeSet* AuraAttributes = Cast<UAuraAttributeSet>(AttributeSet);
	if (!AuraAttributes) return;
	//生命值属性
	OnHealthChanged.Broadcast(AuraAttributes->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributes->GetMaxHealth());

	//魔法值属性
	OnManaChanged.Broadcast(AuraAttributes->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributes->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributes = Cast<UAuraAttributeSet>(AttributeSet);
	if (!AuraAttributes || !AbilitySystemComponent) return;
	//传入想绑定回调的那个属性,具体指定的属性和具体指定的回调函数
	//函数签名要符合要求，才能绑定委托
	//该委托会在属性变化的时候触发广播
	//生命值属性
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributes->GetHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		//广播生命值的新值
		OnHealthChanged.Broadcast(Data.NewValue);
	}
	);
	
	//最大生命值属性
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributes->GetMaxHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		//广播最大生命值的新值
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	}
	);

	//魔力属性
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributes->GetManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		//广播魔法值
		OnManaChanged.Broadcast(Data.NewValue);
	}
	);

	//最大魔力属性
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributes->GetMaxManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		//广播最大魔法值
		OnMaxManaChanged.Broadcast(Data.NewValue);
	}
	);

	//添加lambda让我们可以直接在括号里定义响应委托广播时调用的任意功能,方括号的作用是捕获变量,如果想访问某个成员变量，包含该变量的那个类必须被lambda捕获(一旦我把this放进方括号里，就相当于捕获了当前对象)
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
	[this](const FGameplayTagContainer& AssetTags)
	{
		//在花括号里定义具体要执行的操作
		for (const FGameplayTag& Tag : AssetTags)
		{
			//"Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
			/*如果取一个层级更深的标签，去匹配层级较浅的标签，比如Message，它会返回true*/
			//1.判断传入的Tag是不是消息标签
			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag("Message");
			if (Tag.MatchesTag(MessageTag))
			{
				//把这条数据广播给那些能绑定委托并接收该行数据的 Widget,然后利用数据表行里的资源，在HUD中展示内容
				const FUIWidgetRow* Row = GetDataTableRawByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
				MessageWidgetRowDelegate.Broadcast(*Row);
			}
			
			/*
		//广播标签到Widget Controller
		//当任何效果被应用时我们可以拿到所有和该效果关联的资源标签,这些标签是在游戏效果蓝图里加的，我们可以随心所欲地用它们
		const FString Msg = FString::Printf(TEXT("GE Tag %s"), *Tag.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
		一旦收到这个标签容器我们就会检查里面是否有消息父标签,如果有的话我们就能从数据表里查找对应内容,这意味着我们有一个新的数据表，存放那些资源和UI/数据，就在这里，DT消息数据表
		/我想在控件控制器里添加一个这种类型的变量,这样我们就能随时从这个表里查找，拿到想广播给控件的任何资源
			*/
		}
	}
	);
}

