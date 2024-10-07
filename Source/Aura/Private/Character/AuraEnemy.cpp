// Copyright C&D Games


#include "Character/AuraEnemy.h"

void AAuraEnemy::HighlightActor()
{
	FString Log = GetActorNameOrLabel() + TEXT(" highlighted");
	UE_LOG(LogTemp, Display, TEXT("%s"), * Log);
}

void AAuraEnemy::UnHighlightActor()
{
	FString Log = GetActorNameOrLabel() + TEXT(" unhighlighted");
	UE_LOG(LogTemp, Display, TEXT("%s"), *Log);
}
