// Copyright C&D Games


#include "Character/AuraCharacterBase.h"


AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Weapon Mesh"));
	WeaponMesh->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

