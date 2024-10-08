// Copyright C&D Games


#include "Character/AuraEnemy.h"
#include "Aura/Aura.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraEnemy::AAuraEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributesSet");
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	WeaponMesh->SetRenderCustomDepth(true);

	FString Log = GetActorNameOrLabel() + TEXT(" highlighted");
	UE_LOG(LogTemp, Display, TEXT("%s"), * Log);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	WeaponMesh->SetRenderCustomDepth(false);

	FString Log = GetActorNameOrLabel() + TEXT(" unhighlighted");
	UE_LOG(LogTemp, Display, TEXT("%s"), *Log);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Set custom depth stencil value of the mesh to the same as in PP_Highlight material
	// This, along with SetRenderCustomDepth(true) enables drawing outlines
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}
