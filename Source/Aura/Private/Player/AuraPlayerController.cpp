// Copyright C&D Games


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}


void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	//2 traces for 2 channels, cause we want to also include capsule
	FHitResult CursorHitVis;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHitVis);
	FHitResult CursorHitPawn;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, CursorHitPawn);
	

	// * DECIDING WHICH LINETRACE TO USE
	if (!CursorHitVis.bBlockingHit && !CursorHitPawn.bBlockingHit) return;

	FHitResult& CursorHit = CursorHitVis;

	//if both traces succeed, we use the shorter one
	if (CursorHitVis.bBlockingHit && CursorHitPawn.bBlockingHit) {
		if (CursorHitVis.Distance > CursorHitPawn.Distance) CursorHit = CursorHitPawn;
	}
	else if (CursorHitPawn.bBlockingHit) {
		CursorHit = CursorHitPawn;
	}
	// * END DECIDING WHICH LINETRACE TO USE


	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (!LastActor && ThisActor) {
		ThisActor->HighlightActor();
	}
	else if (LastActor && !ThisActor) {
		LastActor->UnHighlightActor();
	}
	else if (LastActor && ThisActor && ThisActor != LastActor) {
		LastActor->UnHighlightActor();
		ThisActor->HighlightActor();
	}
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>()) {
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}


