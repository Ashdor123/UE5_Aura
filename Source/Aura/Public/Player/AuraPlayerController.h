// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"


class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class USplineComponent;
class UAuraAbilitySystemComponent;


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

	//1.在光标下获取命中结果
	FHitResult CursorHit;

	//按下
	void AbilityInputTagPressed(FGameplayTag InputTag);
	//松开
	void AbilityInputTagReleased(FGameplayTag InputTag);
	//持续按住
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	//同于只转换一次ASC
	UAuraAbilitySystemComponent* GetASC();
	//输入配置
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	//缓存目的地
	FVector CachedDestination = FVector::ZeroVector;
	//跟随时间
	float FollowTime = 0.f;
	//短按时间
	float ShortPressThreshold = 0.5f;
	//布尔值为真，就可以每一帧调用一次添加移动输入
	bool bAutoRunning = false;
	//判断我们锁定了目标嘛？
	bool bTargeting = false;

	//自动运行接受半径
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;
	//样条组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();
};
