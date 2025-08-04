// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "LeCommonRotator.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class LEGACY_API ULeCommonRotator : public UCommonRotator
{
	GENERATED_BODY()
	
public:
	void SetSelectedOptionByText(const FText& InOptionText);
};
