// Ashdor


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

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
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	//俯视角游戏不应该启用控制器的旋转 X,Y,Z
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	/*----------------------------------------------------------------------------------------*/
}
