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
	FGameplayEffectSpecHandle EffectSpecHandle = ASComponent->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveEffectHandle = ASComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && GetRemovalPolicy(EffectSpecHandle.Data.Get()->Def.GetClass()) == EEffectRemovalPolicy::RemoveOnEndOverlap) {
		ActiveEffectHandles.Add(ActiveEffectHandle, ASComponent);
	}
	return true;
}

bool AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	bool bEffectApplied = false;
	for (auto InstantEffect : InstantGameplayEffects) {
		if (InstantEffect.Value == EEffectApplicationPolicy::ApplyOnOverlap) {
			if (ApplyEffectToTarget(TargetActor, InstantEffect.Key))
				bEffectApplied = true;
		}
	}
	for (auto DurationEffect : DurationGameplayEffects) {
		if (DurationEffect.Value == EEffectApplicationPolicy::ApplyOnOverlap) {
			if (ApplyEffectToTarget(TargetActor, DurationEffect.Key))
				bEffectApplied = true;
		}
	}
	for (auto InfiniteEffect : InfiniteGameplayEffects) {
		if (InfiniteEffect.Value.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
			if (ApplyEffectToTarget(TargetActor, InfiniteEffect.Key))
				bEffectApplied = true;
		}
	}
	return bEffectApplied;
}

bool AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	bool bEffectApplied = false;
	for (auto InstantEffect : InstantGameplayEffects) {
		if (InstantEffect.Value == EEffectApplicationPolicy::ApplyOnEndOverlap) {
			if (ApplyEffectToTarget(TargetActor, InstantEffect.Key))
				bEffectApplied = true;
		}
	}
	for (auto DurationEffect : DurationGameplayEffects) {
		if (DurationEffect.Value == EEffectApplicationPolicy::ApplyOnEndOverlap) {
			if (ApplyEffectToTarget(TargetActor, DurationEffect.Key))
				bEffectApplied = true;
		}
	}
	for (auto InfiniteEffect : InfiniteGameplayEffects) {
		if (InfiniteEffect.Value.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
			if (ApplyEffectToTarget(TargetActor, InfiniteEffect.Key))
				bEffectApplied = true;
		}
	}
	for (auto InfiniteEffect : InfiniteGameplayEffects) {
		if (InfiniteEffect.Value.RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
			if (RemoveEffectFromTarget(TargetActor))
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

EEffectApplicationPolicy AAuraEffectActor::GetApplicationPolicy(TSubclassOf<UGameplayEffect> GameplayEffect)
{
	if (InstantGameplayEffects.Contains(GameplayEffect)) {
		auto Policy = InstantGameplayEffects.Find(GameplayEffect);
		return *Policy;
	}
	if (DurationGameplayEffects.Contains(GameplayEffect)) {
		auto Policy = DurationGameplayEffects.Find(GameplayEffect);
		return *Policy;
	}
	if (InfiniteGameplayEffects.Contains(GameplayEffect)) {
		auto Policy = InfiniteGameplayEffects.Find(GameplayEffect);
		return Policy->ApplicationPolicy;
	}
	return EEffectApplicationPolicy::DoNotApply;
}

EEffectRemovalPolicy AAuraEffectActor::GetRemovalPolicy(TSubclassOf<UGameplayEffect> GameplayEffect)
{
	if (InfiniteGameplayEffects.Contains(GameplayEffect)) {
		auto Policy = InfiniteGameplayEffects.Find(GameplayEffect);
		return Policy->RemovalPolicy;
	}
	return EEffectRemovalPolicy::DoNotRemove;
}
