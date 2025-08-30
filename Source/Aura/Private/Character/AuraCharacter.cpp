// Ashdor


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	/*----------------------------------俯视角游戏的基本参数设置------------------------------------------------------*/
	
	//当设置为 true 时，角色会自动旋转使其正面朝向移动方向,适用于俯视角游戏 orient定向 旋转到移动方向
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//角色就会以这个速度朝向移动，至少在偏航方向上, RotationRate 属性调整旋转速度
	GetCharacterMovement()->RotationRate = FRotator(0, 400.f, 0);
	
	//Constrain (限制)到平面
	//角色将被限制在指定的二维平面上移动，无法离开该平面
	//平面约束行为：1.角色只能沿指定的平面移动 2.垂直于平面的移动将被阻止（Z轴移动受限） 3.角色无法跳跃、下蹲或改变高度
	GetCharacterMovement()->bConstrainToPlane = true;
	
	//1.当设置为 true 时，角色在游戏开始时（或生成时）会立即将其位置对齐到最近的约束平面上
	//2.此属性必须配合 bConstrainToPlane = true 使用才有效
	//
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	//俯视角游戏不应该启用控制器的旋转 X,Y,Z
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	/*----------------------------------------------------------------------------------------*/

}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//Init ability actor info for the Server
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	//为客服端初始化角色能力信息
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	if (AuraPlayerState)
	{
		AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
		//设置能力角色信息
		Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
		//初始化角色的能力系统指针与属性集
		AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
		AttributeSet = AuraPlayerState->GetAttributeSet();
	}
	
	/*在什么时候检查空指针，用check，了解玩家控制器是不是在每台机器、每个玩家上都存在这点很重要，
	 *在多人游戏当中，只有在服务器端，所有玩家的玩家控制器才是有效的，
	 *报务器拥有所有玩家的玩家控制器但每个玩家只拥有自己的那个玩家控制器，所以在客户端这台机器上，控制自已角色的那个玩家控制器才是有效的；

	举个例子：比如说，在一个三人游戏中，如果你是客户端玩家，你自己的玩家控制器就是有效，但在你的电脑上，另外两个角色，那两个副本，是没有有效玩家控制器的，在InitAbilityActorInfo（）这个函数调用时，AuraPlayerController就是空指针
	所以在这种情况下，控制器是会出现空指针的情况，故需要检查后往下执行
	*/
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD());
		//我们需要用if语句来判断它是否有效，因为只有本地玩家的HUD才是有效的
		if (AuraHUD)
		{
			AuraHUD->InitOverlay(AuraPlayerController,AuraPlayerState,AbilitySystemComponent,AttributeSet);
			
		}
	}

}


