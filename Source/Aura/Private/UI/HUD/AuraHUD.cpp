// Ashdor


#include "UI/HUD/AuraHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/MenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	//1.判断OverlayWidgetController是否为空指针，为空就创建一个实例，不为空就返回OverlayWidgetController
	if (OverlayWidgetController == nullptr)
	{
		//为OverlayWidgetController赋值（创建） 
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		//设置参数
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		//绑定回调函数
		OverlayWidgetController->BindCallbacksToDependencies();
		//最后设置完成后返回
	}
	
	return OverlayWidgetController;
}

UMenuWidgetController* AAuraHUD::GetMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (MenuWidgetController == nullptr)
	{
		MenuWidgetController = NewObject<UMenuWidgetController>(this,MenuWidgetControllerClass);
		MenuWidgetController->SetWidgetControllerParams(WCParams);
		MenuWidgetController->BindCallbacksToDependencies();
	}
	return MenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	//init overlay会负责创建控件控制器、控件，设置控件的控制器，并把控件添加到界面上
	checkf(OverlayWidgetClass,TEXT("没有初始化OverlayWidgetClass，请在AuraHUD蓝图里设置"));
	checkf(OverlayWidgetControllerClass,TEXT("没有初始化OverlayWidgetControllerClass，请在AuraHUD蓝图里设置"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	//初始化结构体
	const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
	//获取OverlayWidgetController
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	//设置控件控制器
	OverlayWidget->SetWidgetController(WidgetController);
	//在控件控制器上进行广播数据到界面上的控件
	WidgetController->BroadcastInitialValues();
	
	if (Widget)
	{
		Widget->AddToViewport();
	}
}



