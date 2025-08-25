// bong9 All Rights Reserved

#include "MMC_Intelligence.h"

#include "LeAttributeSet.h"
#include "Legacy/Character/LeCharacter.h"

UMMC_Intelligence::UMMC_Intelligence()
{
	IntelligenceDef.AttributeToCapture = ULeAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	IntelligenceDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_Intelligence::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FGameplayEffectContextHandle GameplayEffectContextHandle = Spec.GetContext();
	const ALeCharacter* SourceCharacter = Cast<ALeCharacter>(GameplayEffectContextHandle.GetSourceObject());

	float Intelligence = 10.f;

	return Intelligence * SourceCharacter->CharacterLevel;
}
