// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "LeListDataObject_String.h"
#include "LeListDataObject_StringResolution.generated.h"

/**
 * 
 */
UCLASS()
class LEGACY_API ULeListDataObject_StringResolution : public ULeListDataObject_String
{
	GENERATED_BODY()

public:
	void InitResolutionValues();
	
protected:
	//~ Begin ULeListDataObject_String Interface
	virtual void OnDataObjectInitialized() override;
	//~ End ULeListDataObject_String Interface

private:
	FString ResToValueString(const FIntPoint& InResolution) const;
	FText   ResToDisplayText(const FIntPoint& InResolution) const;

	FString MaximumAllowedResolution;

public:
	FORCEINLINE FString GetMaximumAllowedResolution() const { return MaximumAllowedResolution; }
};
