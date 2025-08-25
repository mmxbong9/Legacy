#include "LeGameplayTags.h"

#include "GameplayTagsManager.h"

FLeGameplayTags FLeGameplayTags::GameplayTags;

void FLeGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Input_TryStartSkillSlot1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.TryStartSkillSlot1"), FString("1"));
	GameplayTags.Input_TryStartSkillSlot2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.TryStartSkillSlot2"), FString("2"));
	GameplayTags.Input_TryStartSkillSlot3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.TryStartSkillSlot3"), FString("3"));

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage"));
}
