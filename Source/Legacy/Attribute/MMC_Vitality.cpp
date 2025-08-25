// bong9 All Rights Reserved

#include "MMC_Vitality.h"

#include "Legacy/Character/LeCharacter.h"

float UMMC_Vitality::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FGameplayEffectContextHandle GameplayEffectContextHandle = Spec.GetContext();
	const ALeCharacter* SourceCharacter = Cast<ALeCharacter>(GameplayEffectContextHandle.GetSourceObject());

	float Vitality = 10.f;

	return Vitality * SourceCharacter->CharacterLevel;
}
