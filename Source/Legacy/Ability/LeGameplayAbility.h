// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LeGameplayAbility.generated.h"

/**
 * @class ULeGameplayAbility
 * @brief 프로젝트 전용 Gameplay Ability 기본 클래스.
 *
 * - UGameplayAbility 를 상속하여 공통 설정 및 편의 기능을 제공
 * - Ability 인스턴싱/Replication 정책 기본값을 통일
 * - Ability 발동 시 캐릭터의 컨트롤러 회전(Yaw) 사용 여부를 제어
 */
UCLASS()
class LEGACY_API ULeGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	ULeGameplayAbility();

	//~ Begin ActivateAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End ActivateAbility Interface

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input)
	FGameplayTagContainer InputTags;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Movement)
	bool bShouldUseControllerRotationYaw;

	UFUNCTION(BlueprintCallable)
	void UseControllerRotationYaw(bool bEnable);
};
