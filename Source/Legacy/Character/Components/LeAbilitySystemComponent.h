#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "LeAbilitySystemComponent.generated.h"

class ULeGameplayAbility;

/**
 * @class ULeAbilitySystemComponent
 * @brief Gameplay Ability System(ASC)을 확장한 커스텀 컴포넌트.
 * 
 * - 커스텀 Ability 등록 및 입력 처리 기능 제공
 * - GameplayTag 기반으로 Ability와 Input을 매핑하여 실행 관리
 */
UCLASS()
class LEGACY_API ULeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	ULeAbilitySystemComponent();

	void AddAbilities(const TArray<TSubclassOf<ULeGameplayAbility>>& Abilities);

	void AbilityInputPressed(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);
};
