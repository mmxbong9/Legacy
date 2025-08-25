// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Intelligence.generated.h"

/**
 * 
 */
UCLASS()
class LEGACY_API UMMC_Intelligence : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_Intelligence();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
};
