// Copyright C&D Games

#pragma once

#include "CoreMinimal.h"

class UGameplayEffect;
class UAbilitySystemComponent;

class AURA_API AbilitySystemUtilities
{
public:
	static void ApplyGameplayEffectToTarget(UAbilitySystemComponent* Source, UAbilitySystemComponent* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level = 1);
	static void ApplyGameplayEffectToSelf(UAbilitySystemComponent* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level = 1);
};
