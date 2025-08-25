// bong9 All Rights Reserved

#include "LeGameplayAbility.h"

#include "Legacy/Character/LeCharacter.h"

ULeGameplayAbility::ULeGameplayAbility()
{
	bShouldUseControllerRotationYaw = false;

	bRetriggerInstancedAbility = false;											// 이미 실행 중인 어빌리티를 다시 활성화 하지 않음
	bServerRespectsRemoteAbilityCancellation = false;							// 클라이언트에서 취소한 어빌리티를 서버가 반드시 따르지 않음
	ReplicationPolicy  = EGameplayAbilityReplicationPolicy::ReplicateNo;		// 로컬 전용 어빌리티
	InstancingPolicy   = EGameplayAbilityInstancingPolicy::InstancedPerActor;	// 각 액터별 인스턴스 보유 하고, 각자 동작
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;	// 로컬 예측. 클라 먼저 실행 후 서버 검증
	NetSecurityPolicy  = EGameplayAbilityNetSecurityPolicy::ClientOrServer;		// 클라, 서버 둘다 실행 가능
}

void ULeGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bShouldUseControllerRotationYaw ? UseControllerRotationYaw(true) : UseControllerRotationYaw(false);
}

void ULeGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULeGameplayAbility::UseControllerRotationYaw(bool bEnable)
{
	if (ALeCharacter* Character = Cast<ALeCharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->bUseControllerRotationYaw = bEnable;
	}
}
