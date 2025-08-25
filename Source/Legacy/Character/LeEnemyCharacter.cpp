// bong9 All Rights Reserved

#include "LeEnemyCharacter.h"

#include "Components/CombatComponent.h"
#include "Legacy/Attribute/LeAttributeSet.h"

ALeEnemyCharacter::ALeEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<ULeAbilitySystemComponent>(GET_MEMBER_NAME(AbilitySystemComponent));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<ULeAttributeSet>(GET_MEMBER_NAME(AttributeSet));
}

void ALeEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeAbilitySystem();
	AddAbilities();
}

void ALeEnemyCharacter::InitializeAbilitySystem()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	if (GetCombatComponent()) { GetCombatComponent()->BindTagChangedCallbacks(); }
	if (HasAuthority()) { InitializeDefaultAttributes(); }
}
