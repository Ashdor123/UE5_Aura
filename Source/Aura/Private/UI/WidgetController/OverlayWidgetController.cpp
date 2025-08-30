// Ashdor


#include "UI/WidgetController/OverlayWidgetController.h"

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
	//生命值属性
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributes->GetHealthAttribute()).AddUObject(this,&UOverlayWidgetController::HealthChanged);
	
	//最大生命值属性
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributes->GetMaxHealthAttribute()).AddUObject(this,&UOverlayWidgetController::MaxHealthChanged);

	//魔力属性
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributes->GetManaAttribute()).AddUObject(this,&UOverlayWidgetController::ManaChanged);

	//最大魔力属性
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributes->GetMaxManaAttribute()).AddUObject(this,&UOverlayWidgetController::MaxManaChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	//广播生命值的新值
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	//广播最大生命值的新值
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
