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

bool AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	bool bEffectApplied = false;
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		if(ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass))
			bEffectApplied = true;
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		if(ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass))
			bEffectApplied = true;
	}
	return bEffectApplied;
}

bool AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	bool bEffectApplied = false;
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
		if(ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass))
			bEffectApplied = true;
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
		if(ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass))
			bEffectApplied = true;
	}
	return bEffectApplied;
}
