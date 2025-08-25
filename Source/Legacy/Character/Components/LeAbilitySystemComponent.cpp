// bong9 All Rights Reserved

#include "LeAbilitySystemComponent.h"
#include "Legacy/Ability/LeGameplayAbility.h"

ULeAbilitySystemComponent::ULeAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULeAbilitySystemComponent::AddAbilities(const TArray<TSubclassOf<ULeGameplayAbility>>& Abilities)
{
	for (auto AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const ULeGameplayAbility* Ability = Cast<ULeGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AppendTags(Ability->InputTags);
			GiveAbility(AbilitySpec);
		}
	}
}

void ULeAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.GetDynamicSpecSourceTags().HasTag(InputTag))
		{
			AbilitySpecInputPressed(Spec);

			if (!Spec.IsActive())
			{
				UE_LOG(LogTemp, Warning, TEXT("Ability InputPressed"));
				TryActivateAbility(Spec.Handle);
			}
		}
	}
}

void ULeAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.GetDynamicSpecSourceTags().HasTag(InputTag))
		{
			UE_LOG(LogTemp, Warning, TEXT("Ability InputReleased"));
			AbilitySpecInputReleased(Spec);
		}
	}
}
