// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();
	
	//从敌人接口抽象类中重写的纯虚函数
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//END

	/** Combat Interface */
	virtual int32 GetPlayerLevel() override;

	/**End Combat Interface */
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	//敌人的等级不用复制，只在服务端关注等级，也就是说只有服务器会执行那些需要用等级做计算的重要操作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
};
