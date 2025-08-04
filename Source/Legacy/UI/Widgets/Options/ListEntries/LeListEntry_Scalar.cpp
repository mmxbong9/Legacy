// bong9 All Rights Reserved


#include "LeListEntry_Scalar.h"

#include "Legacy/UI/Widgets/Options/DataObjects/LeListDataObject_Scalar.h"
#include "AnalogSlider.h"

void ULeListEntry_Scalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SettingSlider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::OnSliderValueChanged);
	SettingSlider->OnMouseCaptureBegin.AddUniqueDynamic(this, &ThisClass::OnMouseCaptureBegin);
}

void ULeListEntry_Scalar::OnOwningListDataObjectSet(ULeListDataObject* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningScalarDataObject = CastChecked<ULeListDataObject_Scalar>(InOwningListDataObject);

	NumericSettingValue->SetNumericType(CachedOwningScalarDataObject->GetDisplayNumericType());
	NumericSettingValue->FormattingSpecification = CachedOwningScalarDataObject->GetNumberFormattingOptions();
	NumericSettingValue->SetCurrentValue(CachedOwningScalarDataObject->GetCurrentValue());

	SettingSlider->SetMinValue(CachedOwningScalarDataObject->GetDisplayValueRange().GetLowerBoundValue());
	SettingSlider->SetMaxValue(CachedOwningScalarDataObject->GetDisplayValueRange().GetUpperBoundValue());
	SettingSlider->SetStepSize(CachedOwningScalarDataObject->GetSliderStepSize());
	SettingSlider->SetValue(CachedOwningScalarDataObject->GetCurrentValue());
}

void ULeListEntry_Scalar::OnOwningListDatObjectModified(ULeListDataObject* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	Super::OnOwningListDatObjectModified(OwningModifiedData, ModifyReason);

	if (CachedOwningScalarDataObject)
	{
		NumericSettingValue->SetCurrentValue(CachedOwningScalarDataObject->GetCurrentValue());
		SettingSlider->SetValue(CachedOwningScalarDataObject->GetCurrentValue());
	}
}

void ULeListEntry_Scalar::OnSliderValueChanged(float Value)
{
	if (CachedOwningScalarDataObject)
	{
		CachedOwningScalarDataObject->SetCurrentValueFromSlider(Value);
	}
}

void ULeListEntry_Scalar::OnMouseCaptureBegin()
{
	SelectThisEntryWidget();
}