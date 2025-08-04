// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "LeListDataObject.h"
#include "LeListDataObject_Value.generated.h"

class FOptionsDataInteractionHelper;

/**
 * 
 */
UCLASS(Abstract)
class LEGACY_API ULeListDataObject_Value : public ULeListDataObject
{
	GENERATED_BODY()

public:
	void SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGetter);
	void SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSetter);

	void SetDefaultValueFromString(const FString& InDefaultValue) { DefaultStringValue = InDefaultValue; };

	//~ Begin ULeListDataObject Interface
	virtual bool HasDefaultValue() const override { return DefaultStringValue.IsSet(); }
	//~ End ULeListDataObject Interface
	
protected:
	FString GetDefaultValueAsString() const { return DefaultStringValue.GetValue(); }
	
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicGetter;
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicSetter;

private:
	TOptional<FString> DefaultStringValue;
};
