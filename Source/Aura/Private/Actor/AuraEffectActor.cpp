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
	FActiveGameplayEffectHandle ActiveEffectHandle = ASComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
		ActiveEffectHandles.Add(ActiveEffectHandle, ASComponent);
	}
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
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		if (ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass))
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
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
		if (ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass))
			bEffectApplied = true;
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
		if (RemoveEffectFromTarget(TargetActor)) {
			bEffectApplied = true;
		}
	}
	return bEffectApplied;
}

bool AAuraEffectActor::RemoveEffectFromTarget(AActor* TargetActor)
{
	UAbilitySystemComponent* ASComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (!IsValid(ASComponent)) return false;
	bool bEffectRemoved = false;
	TArray<FActiveGameplayEffectHandle> HandlesToRemove;
	for (auto HandlePair : ActiveEffectHandles) {
		if (HandlePair.Value == ASComponent) {
			ASComponent->RemoveActiveGameplayEffect(HandlePair.Key, 1);
			HandlesToRemove.Add(HandlePair.Key);
			bEffectRemoved = true;
		}
	}
	for (auto HandleToRemove : HandlesToRemove) {
		ActiveEffectHandles.Remove(HandleToRemove);
	}
	return bEffectRemoved;
}
