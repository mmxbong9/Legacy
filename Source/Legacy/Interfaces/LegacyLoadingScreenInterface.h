// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LegacyLoadingScreenInterface.generated.h"

UINTERFACE(BlueprintType, MinimalAPI)
class ULegacyLoadingScreenInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEGACY_API ILegacyLoadingScreenInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void OnLoadingScreenActivated();
	virtual void OnLoadingScreenActivated_Implementation() = 0;

	UFUNCTION(BlueprintNativeEvent)
	void OnLoadingScreenDeactivated();
	virtual void OnLoadingScreenDeactivated_Implementation() = 0;
};
