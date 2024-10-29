// Copyright C&D Games


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

bool AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* ASComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!ASComponent) return false;
	check(GameplayEffectClass);

	FGameplayEffectContextHandle EffectContextHandle = ASComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = ASComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContextHandle);
	ASComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	return true;
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{

}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{

}
