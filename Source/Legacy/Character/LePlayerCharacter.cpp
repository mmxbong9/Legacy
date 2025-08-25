// bong9 All Rights Reserved

#include "LePlayerCharacter.h"

#include "Components/CombatComponent.h"
#include "Legacy/PlayerState/LePlayerState.h"

ALePlayerCharacter::ALePlayerCharacter()
{
	// 실시간으로 본 위치 갱신을 해야 충돌 체크가 항상 가능하다.
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

void ALePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Server
	InitializeAbilitySystem();

	AddAbilities();
}

void ALePlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Client
	InitializeAbilitySystem();
}

void ALePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ALePlayerCharacter::InitializeAbilitySystem()
{
	ALePlayerState* LePlayerState = GetPlayerState<ALePlayerState>();
	check(LePlayerState);
	LePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	AbilitySystemComponent = LePlayerState->GetAbilitySystemComponent();
	AttributeSet = LePlayerState->GetAttributeSet();
	if (GetCombatComponent()) { GetCombatComponent()->BindTagChangedCallbacks(); }
	if (HasAuthority()) { InitializeDefaultAttributes(); }	
}