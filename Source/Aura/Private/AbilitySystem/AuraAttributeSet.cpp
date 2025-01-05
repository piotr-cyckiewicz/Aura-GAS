// Copyright C&D Games


#include "AbilitySystem/AuraAttributeSet.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"


UAuraAttributeSet::UAuraAttributeSet() {
	InitMaxHealth(100);
	InitHealth(50);
	InitMaxMana(100);
	InitMana(50);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	//Vital attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);


	//Primary attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	/*else if (Attribute == GetMaxHealthAttribute()) {
		SetHealth(FMath::Clamp(GetHealth(), 0, NewValue));
	}*/
	else if (Attribute == GetManaAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
	/*else if (Attribute == GetMaxManaAttribute()) {
		SetMana(FMath::Clamp(GetMana(), 0, NewValue));
	}*/
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties;
	SetGameplayEffectProperties(Data, EffectProperties);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute()) {
		SetMana(FMath::Clamp(GetMana(), 0, GetMaxMana()));
	}
}

void UAuraAttributeSet::SetGameplayEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& GameplayEffectProperties)
{
	const FGameplayEffectContextHandle ContextHandle = Data.EffectSpec.GetContext();
	GameplayEffectProperties.ContextHandle = ContextHandle;

	const auto SourceASC = ContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	GameplayEffectProperties.SourceASC = SourceASC;

	if (IsValid(SourceASC) && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->AvatarActor.IsValid()) {
		AActor* SourceAvatar = SourceASC->AbilityActorInfo->AvatarActor.Get();
		GameplayEffectProperties.SourceAvatar = SourceAvatar;

		AController* SourceController = SourceASC->AbilityActorInfo->PlayerController.Get();
		if (!SourceController && SourceAvatar) {
			if (const APawn* Pawn = Cast<APawn>(SourceAvatar)) {
				SourceController = Pawn->GetController();
			}
		}
		if (SourceController) {
			GameplayEffectProperties.SourceController = SourceController;
			ACharacter* SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());
			GameplayEffectProperties.SourceCharacter = SourceCharacter;
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()) {
		AActor* TargetAvatar = Data.Target.AbilityActorInfo->AvatarActor.Get();
		GameplayEffectProperties.TargetAvatar = TargetAvatar;
		AController* TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		GameplayEffectProperties.TargetController = TargetController;
		ACharacter* TargetCharacter = Cast<ACharacter>(TargetAvatar);
		GameplayEffectProperties.TargetCharacter = TargetCharacter;
		GameplayEffectProperties.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetAvatar);
	}
}


//
// Vital Attributes
//
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}
void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}
void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

//
// Primary Attributes
//
void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}
void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}
void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}