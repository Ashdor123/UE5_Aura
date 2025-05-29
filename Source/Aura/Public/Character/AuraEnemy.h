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
protected:
	
	
};
