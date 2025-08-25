// bong9 All Rights Reserved

#include "LePlayerState.h"

#include "AbilitySystemComponent.h"
#include "Legacy/Attribute/LeAttributeSet.h"
#include "Legacy/Character/Components/LeAbilitySystemComponent.h"
#include "Legacy/Global/GlobalUtil.h"

ALePlayerState::ALePlayerState()
{
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<ULeAbilitySystemComponent>(GET_MEMBER_NAME(AbilitySystemComponent));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<ULeAttributeSet>(GET_MEMBER_NAME(AttributeSet));
}
