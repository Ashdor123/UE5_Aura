// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();
	//在玩家tick中执行追踪，并对碰到的实现了敌人接口的角色进行高亮处理
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	//UInputMappingContext 需要提前声明的变量 并且还需要添加模块 包含这些Enhanced.Input类的头文件的EnhancedInput模块
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);
	
	//设置每帧追踪光标的函数
	void CursorTrace();
	
	//定义2个Enemy接口指针，一个指向上一帧，一个指向现在的帧
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
};
