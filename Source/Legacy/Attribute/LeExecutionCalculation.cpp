// bong9 All Rights Reserved

#include "LeExecutionCalculation.h"

#include "LeAttributeSet.h"
#include "Legacy/GameplayTags/LeGameplayTags.h"

struct FCtDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalAttack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	FCtDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(ULeAttributeSet, PhysicalAttack,     Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(ULeAttributeSet, PhysicalResistance, Target, true);
	}
};

static const FCtDamageStatics DamageStatics()
{
	static FCtDamageStatics Statics;
	return Statics;
}

ULeExecutionCalculation::ULeExecutionCalculation()
{
	const auto& Statics = DamageStatics();
	RelevantAttributesToCapture.Add(Statics.PhysicalAttackDef);
	RelevantAttributesToCapture.Add(Statics.PhysicalResistanceDef);
}

void ULeExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const auto& Statics = DamageStatics();

	FAggregatorEvaluateParameters Params;
	Params.SourceTags = ExecutionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
	Params.TargetTags = ExecutionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();;
	
	float SourcePhysicalAttack = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Statics.PhysicalAttackDef, Params, SourcePhysicalAttack);
	SourcePhysicalAttack = FMath::Max<float>(SourcePhysicalAttack, 0.f);

	float TargetPhysicalResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Statics.PhysicalResistanceDef, Params, TargetPhysicalResistance);
	TargetPhysicalResistance = FMath::Max<float>(TargetPhysicalResistance, 0.f);
	
	float AbilityDamage = Spec.GetSetByCallerMagnitude(FLeGameplayTags::Get().Damage);
	float EffectiveDamage = (SourcePhysicalAttack + AbilityDamage) - TargetPhysicalResistance;
	
	float Damage = EffectiveDamage;
	
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(ULeAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage));
}
