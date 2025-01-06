// Copyright C&D Games

#include "AbilitySystem/Utilities/AbilitySystemUtilities.h"
#include "AbilitySystemComponent.h"

void AbilitySystemUtilities::ApplyGameplayEffectToTarget(UAbilitySystemComponent* Source, UAbilitySystemComponent* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level)
{
	check(IsValid(Source));
	check(IsValid(Target));
	check(IsValid(GameplayEffectClass));

	const FGameplayEffectContextHandle ContextHandle;
	const FGameplayEffectSpecHandle SpecHandle = Source->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	Source->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Target);
}

void AbilitySystemUtilities::ApplyGameplayEffectToSelf(UAbilitySystemComponent* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level)
{
	check(IsValid(Target));
	check(IsValid(GameplayEffectClass));

	const FGameplayEffectContextHandle ContextHandle;
	const FGameplayEffectSpecHandle SpecHandle = Target->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	Target->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Target);
}