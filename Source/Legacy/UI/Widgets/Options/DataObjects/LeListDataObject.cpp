// bong9 All Rights Reserved

#include "LeListDataObject.h"

#include "Legacy/Settings/LegacyGameUserSettings.h"

void ULeListDataObject::InitDataObject()
{
	OnDataObjectInitialized();
}

void ULeListDataObject::AddEditCondition(const FOptionsDataEditConditionDescriptor& InEditCondition)
{
	EditConditionDescArray.Add(InEditCondition);
}

void ULeListDataObject::AddEditDependencyData(ULeListDataObject* InDependencyData)
{
	if (!InDependencyData->OnListDataModified.IsBoundToObject(this))
	{
		InDependencyData->OnListDataModified.AddUObject(this, &ThisClass::OnEditDependencyDataModified);
	}
}

bool ULeListDataObject::IsDataCurrentlyEditable()
{
	bool bIsEditable = true;
	
	if (EditConditionDescArray.IsEmpty())
	{
		return bIsEditable;
	}

	FString CachedDisabledRichReason;

	for (const FOptionsDataEditConditionDescriptor& Condition : EditConditionDescArray)
	{
		if (!Condition.IsValid() || Condition.IsEditConditionMet())
		{
			continue;
		}

		bIsEditable = false;

		CachedDisabledRichReason.Append(Condition.GetDisableRichReason());

		SetDisabledRichText(FText::FromString(CachedDisabledRichReason));

		if (Condition.HasForcedStringValue())
		{
			const FString ForcedStringValue = Condition.GetDisabledForcedStringValue();
			
			// if the current value this data object has can be set to the forced value
			if (CanSetToForcedStringValue(ForcedStringValue))
			{
				OnSetToForcedStringValue(ForcedStringValue);
			}
		}
	}

	return bIsEditable;
}

void ULeListDataObject::OnDataObjectInitialized()
{
	
}

void ULeListDataObject::NotifyListDataModified(ULeListDataObject* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);

	if (bShouldApplyChangeImmediatly)
	{
		ULegacyGameUserSettings::Get()->ApplySettings(true);
	}
}

void ULeListDataObject::OnEditDependencyDataModified(ULeListDataObject* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason)
{
	OnDependentDataModified.Broadcast(ModifiedDependencyData, ModifyReason);
}
