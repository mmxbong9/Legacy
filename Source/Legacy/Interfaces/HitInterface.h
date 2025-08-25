// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

UINTERFACE()
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEGACY_API IHitInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PushActor(const FVector& Direction, const float Strength);
};
