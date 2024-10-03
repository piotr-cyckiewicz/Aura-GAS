// Copyright C&D Games


#include "Character/AuraCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"


AAuraCharacter::AAuraCharacter()
{
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("Camera Spring Arm"));
	CameraSpringArm->SetupAttachment(RootComponent);

	CameraSpringArm->SetRelativeRotation(FRotator(-45, 0, 0));
	CameraSpringArm->bInheritPitch = false;
	CameraSpringArm->bInheritRoll = false;
	CameraSpringArm->bInheritYaw = false;
	CameraSpringArm->TargetArmLength = 800.0f;
	CameraSpringArm->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(CameraSpringArm);
	Camera->bUsePawnControlRotation = false;


	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}
