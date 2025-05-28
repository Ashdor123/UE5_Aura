// Ashdor


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
AAuraPlayerController::AAuraPlayerController()
{
	//Replication 复制就是响应服务器上的数据更新，并将这些更新发送给客户端
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//检查会在条件不满足的时候停止执行
	check(AuraContext);
	//1.类型为增强输入本地玩家子系统的本地指针子系统,这就是我们添加映射上下文的方法，让玩家控制器能够获取数据
	//2.GetLocalPlayer() 获取当前本地玩家对象  GetSubsystem<...>() 模板函数获取指定的子系统实例
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	//参数0表示优先级（数字越大优先级越高）
	Subsystem->AddMappingContext(AuraContext,0);
	
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

	UEnhancedInputComponent* EnhancedInputComponent =CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
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
