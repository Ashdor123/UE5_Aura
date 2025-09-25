// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "AuraWidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY();

	FWidgetControllerParams() {}
	
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS) :
	PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}
	
	//玩家控制器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	//玩家状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
	//能力系统组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	//属性集
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
public:
	//通过我们构建的结构体初始化4个数据信息
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	//任何继承了这个函数的小部件控制器都能广播自己独有的初始值
	//BroadcastInitialValues 广播初始值
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	//Dependencies依赖关系
	//用处：属性回调绑定到所有这些依赖项上
	virtual void BindCallbacksToDependencies();
protected:

	//下面的4个变量将用于获取数据，并向我们的控件广播
	//玩家控制器
	UPROPERTY(BlueprintReadWrite, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	//玩家状态
	UPROPERTY(BlueprintReadWrite, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	//能力系统组件
	UPROPERTY(BlueprintReadWrite, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	//属性集
	UPROPERTY(BlueprintReadWrite, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
