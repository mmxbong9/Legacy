#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FLeGameplayTags
{
public:
	static const FLeGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Input_TryStartSkillSlot1;
	FGameplayTag Input_TryStartSkillSlot2;
	FGameplayTag Input_TryStartSkillSlot3;
	
	FGameplayTag Damage;

private:
	static FLeGameplayTags GameplayTags;
};
