// Copyright C&D Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveGameplayEffectHandle.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8 {
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8 {
	RemoveOnOverlap,
	RemoveOnEndOverlap,
	DoNotRemove
};

USTRUCT(BlueprintType)
struct FEffectPolicyPair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	EEffectApplicationPolicy ApplicationPolicy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	EEffectRemovalPolicy RemovalPolicy;
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();


protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	bool ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	bool OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	bool OnEndOverlap(AActor* TargetActor);

	bool RemoveEffectFromTarget(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	EEffectApplicationPolicy GetApplicationPolicy(TSubclassOf<UGameplayEffect> GameplayEffect);
	UFUNCTION(BlueprintCallable)
	EEffectRemovalPolicy GetRemovalPolicy(TSubclassOf<UGameplayEffect> GameplayEffect);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	bool bDestroyOnEffectRemoval = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TMap<TSubclassOf<UGameplayEffect>, EEffectApplicationPolicy> InstantGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TMap<TSubclassOf<UGameplayEffect>, EEffectApplicationPolicy> DurationGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TMap<TSubclassOf<UGameplayEffect>, FEffectPolicyPair> InfiniteGameplayEffects;
	
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
};
