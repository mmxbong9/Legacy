// bong9 All Rights Reserved

#pragma once

#include <rapidjson/document.h>

#include "CoreMinimal.h"
#include "LeListEntry.h"
#include "LeListEntry_Scalar.generated.h"

class ULeListDataObject_Scalar;
class UCommonNumericTextBlock;
class UAnalogSlider;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class LEGACY_API ULeListEntry_Scalar : public ULeListEntry
{
	GENERATED_BODY()

protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

	//~ Begin ULeListEntry Interface
	virtual void OnOwningListDataObjectSet(ULeListDataObject* InOwningListDataObject) override;
	virtual void OnOwningListDatObjectModified(ULeListDataObject* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;
	//~ End ULeListEntry Interface

private:
	UFUNCTION()
	void OnSliderValueChanged(float Value);

	UFUNCTION()
	void OnMouseCaptureBegin();
	
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true))
	TObjectPtr<UCommonNumericTextBlock> NumericSettingValue;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true))
	TObjectPtr<UAnalogSlider> SettingSlider; 
	//***** Bound Widgets *****//

	UPROPERTY(Transient)
	TObjectPtr<ULeListDataObject_Scalar> CachedOwningScalarDataObject;
};
