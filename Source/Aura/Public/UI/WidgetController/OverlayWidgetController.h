// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"


//这是数据表行结构基类
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	//1.创建一个用来显示屏幕上信息的标签
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AssetTag = FGameplayTag();
	//2.创建一个显示的消息（向用户显示文本，我们用FText)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();
	/*3.现在在这张数据表中，我想创建一个可以显示在屏幕上的小部件类，
	 *因此，对于任何特定的游戏玩法标签，我们都能创建一个小部件，并在接收到这个标签对应的游戏效果时，将其添加到视口中显示*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAuraUserWidget> MessageWidget;
	//4.图像
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

class UAuraUserWidget;
//声明了一个能广播浮点数的委托，这个委托是专门针对生命值设计的
//Signature 签名
//1.生命值
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
//2.魔法值
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);



/**
 * 
 */

//蓝图类型意味着我们可以在事件图中把它当作一种类型使用，比如执行类型转换
//蓝图化意味着我们可以基于这个类创建蓝图
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	//FOnHealthChangedSignature 委托类型
	//OnHealthChanged 委托对象
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnManaChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxManaChangedSignature OnMaxManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;
protected:
	//数据表
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	//函数签名要符合要求，才能绑定委托
	//该委托会在属性变化的时候触发广播
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;

	void ManaChanged(const FOnAttributeChangeData& Data) const;
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;

	//这个函数的设计目标是无论行类型如何，都能找到数据表中的某一行并返回它
	template<typename T>
	T* GetDataTableRawByTag(UDataTable* DataTable, const FGameplayTag Tag);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRawByTag(UDataTable* DataTable, const FGameplayTag Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),  TEXT(""));;
}
