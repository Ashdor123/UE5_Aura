// Ashdor


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	//调整敌人的网格体的可见性响应，为阻挡
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECR_Block);
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	
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
