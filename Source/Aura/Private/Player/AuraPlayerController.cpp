// Ashdor


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	//Replication 复制就是响应服务器上的数据更新，并将这些更新发送给客户端
	bReplicates = true;
	LastActor = nullptr;
	ThisActor = nullptr;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");

}

void AAuraPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Cursor 光标
	CursorTrace();

	//处理鼠标左键短按的自动移动
	AutoRun();
	
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	
	if (APawn* ControlledPawn = GetPawn())
	{
		//这个向量是路径上最接近控制角色的位置
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		//向量方向
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		//移动位置
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination < AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::CursorTrace()
{
	
	//2.获取碰撞结果，就是光标的命中点
	//PS：这里要给敌人的碰撞预设中的可见性设为阻挡
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,CursorHit);
	//3.判断是否命中,
	if (!CursorHit.bBlockingHit) return;
	//4.如果转换成功，那么被命中的Actor就实现了IEnemy接口
	// Ps:当Actor没有实先IEnemy接口，转换会返回一个空指针， 我们可以利用这个信息,所以我们可以定义2个指针，一个指向上一帧的，一个是现在的这一帧的，来判断它们是否是相同的对象
	LastActor= ThisActor;
	//5.如果转换可能失败，这意味着ThisActor会变成空指针，而LastActor会保存更新前的ThisActor值
	//换句话说，LastActor会保留上一帧的情况
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	
	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}
	
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		//ThisActor的有效性
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
	
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputReleased(InputTag);
		}
		return;
	}

	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputReleased(InputTag);
		}
	}
	else
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			//创建一条跟随的路径点(导航路径)   同步查找位置路径(FindPathToLocationSynchronously)
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,ControlledPawn->GetActorLocation(), CachedDestination))
			{
				//我们要处理的点
				//NavPath->PathPoints

				//每次设置样条点前，先把之前的点都清空
				Spline->ClearSplinePoints();
				//接着遍历路径
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					//在样条曲线上加一点
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					//DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
				}
				//缓存最后一个导航路径点，避免点击无法前往的区域而不可控
				if (NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				}
				
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//如果不是左鼠标键，比如是数字键1或右鼠标键,我们就会检查是否激活了能力，如果没激活，就不执行
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputHeld(InputTag);
		}
		return;
	}

	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputHeld(InputTag);
		}
	}
	else //这段代码会处理鼠标按下时的移动，且只有在不锁定目标的情况可以
	{
		//处理点击移动
		//1.首先累加FollowTime，只要按着左键鼠标按钮，就把每帧时间增量加到跟随时间里
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		if (CursorHit.bBlockingHit)
		{
			//如果成功了,我们就在hit结果里缓存目标位置，把缓存目标设为hit.Location
			CachedDestination = CursorHit.ImpactPoint;
		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
		
	}
	
	return AuraAbilitySystemComponent;
}



void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//检查会在条件不满足的时候停止执行
	check(AuraContext);
	//1.类型为增强输入本地玩家子系统的本地指针子系统,这就是我们添加映射上下文的方法，让玩家控制器能够获取数据
	//2.GetLocalPlayer() 获取当前本地玩家对象  GetSubsystem<...>() 模板函数获取指定的子系统实例
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	//只有在本地控制的机器并且有有效的localplayer时，subsystem才会有效
	if (Subsystem)
	{
		check(Subsystem);
		//参数0表示优先级（数字越大优先级越高）
		Subsystem->AddMappingContext(AuraContext,0);
	}

	
	//显示鼠标的光标
	bShowMouseCursor = true;
	// 使用默认光标样式
	DefaultMouseCursor = EMouseCursor::Default;
	// 配置输入模式(RPG)
	FInputModeGameAndUI InputModeData;
	// 不锁定鼠标到窗口
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// 操作时不隐藏光标
	InputModeData.SetHideCursorDuringCapture(false);
	// 应用配置
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent =CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);

	//按下,这里调用的移动输入
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	
	
}

//处理移动的函数
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	//获取控制器的旋转
	const FRotator Rotation = GetControlRotation();
	//除了Y轴，其它轴长度为0  获取角色偏航旋转（通常忽略俯仰和翻滚）
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);
	
	//获取前向向量的旋转,X轴将为我们的前方
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//获取向右的向量,右轴向量
	//GetUnitAxis返回的是单位向量，也就是归一了
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	//有了方向后，就可以为棋子添加移动输入了
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		//前后移动
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		//左右移动
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
		
	}

}




