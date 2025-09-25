// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	//1.定义一个Get函数，返回这个具体类型
	static UAuraAssetManager& Get();

protected:
	//2.很早阶段就会调用的一个函数 -- 作为游戏资源初始加载的时机
	virtual void StartInitialLoading() override;
};
