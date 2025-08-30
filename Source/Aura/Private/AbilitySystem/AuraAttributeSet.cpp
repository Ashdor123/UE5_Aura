// Ashdor


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	//用宏初始化属性值
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(25.f);
	InitMaxMana(50.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//5.注册健康值并复制
	//DOREPLIFETIME_CONDITION_NOTIFY 重复生命周期条件通知
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
}

//现有生命值
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	//3.让游戏能力系统知道健康值刚刚被同步了的宏定义
	//GAMEPLAYATTRIBUTE_REPNOTIFY 游戏玩法属性复制通知
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Health,OldHealth);
}
//最大生命值
void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxHealth,OldMaxHealth);
}
//现有魔力
void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	//游戏玩法属性复制通知 GAMEPLAYATTRIBUTE_REPNOTIFY
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxMana,OldMaxMana);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	//预属性改变指的是我们可以调整属性当前值的变动,这个函数会在属性变化真正生效前被调用
	//预属性改变会在属性发生变化时触发,无论是通过属性访问器，比如我们利用宏生成的setter函数或者是因为游戏效果，任何改变属性的行为都会触发这个函数
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
		
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
		
	}

}

void UAuraAttributeSet::SetEffectProperties(const struct FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	//Extension，Magnitude，EvaluatedData 扩展，幅度，评估数据

	// if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Health:%f"),GetHealth());
	// 	UE_LOG(LogTemp, Warning, TEXT("Magnitude:%f"),Data.EvaluatedData.Magnitude);
	// }

	//来源 = 效果的触发者， 目标 = 效果的目标（该属性集的拥有者），被影响的对象
	
	//1.获取效果上下文
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	
	//2.通过效果上下文获取能力系统组件
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	//3.获取化身角色（Avatar） 可以直接GetAvatar
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		
		//4.从角色信息里获取控制器
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		//如果通过4获取到的控制器为空，可以通过Pawn获取
		if (Props.SourceAvatarActor != nullptr && Props.SourceController == nullptr )
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		//5.获取源角色
		if (Props.SourceController)
		{
			Props.SourceCharacter = Props.SourceController->GetCharacter();
				
		}

		//6.获取目标角色
		if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
		{
			Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
			
			//7.获取目标控制器
			Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
			
			//8.获取目标角色
			Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);

			//9.获取目标能力系统组件
			Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
		}
		
	}
}


//在游戏效果修改属性之后执行
void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);
	
}

