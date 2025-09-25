// Ashdor

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
public:
	AAuraCharacter();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;
	
	/*Combat Interface*/
	virtual int32 GetPlayerLevel() override;
	
	/*End Combat Interface*/
private:
	//初始化角色能力信息的函数
	virtual void InitAbilityActorInfo() override;
};
