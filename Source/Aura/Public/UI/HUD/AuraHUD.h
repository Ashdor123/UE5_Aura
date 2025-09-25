// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UMenuWidgetController;
class UAttributeSet;
class UAuraUserWidget;
class UOverlayWidgetController;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:
	
	
	//设置OverlayWidgetController
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	//设置MenuWidgetController
	UMenuWidgetController* GetMenuWidgetController(const FWidgetControllerParams& WCParams);

	//初始化Overlay控件中的结构体的参数
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	//控件类型
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	//菜单控件控制器
	UPROPERTY()
	TObjectPtr<UMenuWidgetController> MenuWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMenuWidgetController> MenuWidgetControllerClass;
};
