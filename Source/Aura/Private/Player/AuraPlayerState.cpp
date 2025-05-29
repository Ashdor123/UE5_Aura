// Ashdor


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	//使用 UAuraAbilitySystemComponent 而非引擎默认的 UAbilitySystemComponent 允许：1.添加项目特定的技能逻辑 2.实现自定义的伤害计算 3.创建特殊的效果交互系统
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	//设置能力系统组件可以复制
	//当设置为 true 时，该组件会在网络游戏中被复制到所有客户端
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	//网络更新频率 = 这是服务器尝试更新客服端的频率
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
