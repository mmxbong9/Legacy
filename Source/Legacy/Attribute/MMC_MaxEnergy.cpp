// bong9 All Rights Reserved

#include "MMC_MaxEnergy.h"

#include "LeAttributeSet.h"

UMMC_MaxEnergy::UMMC_MaxEnergy()
{
	IntelligenceDef.AttributeToCapture = ULeAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	IntelligenceDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxEnergy::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	
	float MaxEnergy = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, MaxEnergy);
	
	return MaxEnergy * 5.f;
}
