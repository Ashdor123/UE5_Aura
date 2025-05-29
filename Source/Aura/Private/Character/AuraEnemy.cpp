// Ashdor


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	//调整敌人的网格体的可见性响应，为阻挡
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECR_Block);
	//使用 UAuraAbilitySystemComponent 而非引擎默认的 UAbilitySystemComponent 允许：1.添加项目特定的技能逻辑 2.实现自定义的伤害计算 3.创建特殊的效果交互系统
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	//设置能力系统组件可以复制
	//当设置为 true 时，该组件会在网络游戏中被复制到所有客户端
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
}

void AAuraEnemy::HighlightActor()
{
	//开启敌人的角色轮廓显示
	//Render渲染 调用网格体->设置自定义渲染深度
	GetMesh()->SetRenderCustomDepth(true);
	//Stencil模版 调用网格体->设置自定义深度模板值
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	//开启武器的角色轮廓显示
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	//关闭敌人的角色轮廓显示
	GetMesh()->SetRenderCustomDepth(false);
	//关闭敌人的武器轮廓显示
	Weapon->SetRenderCustomDepth(false);
}
