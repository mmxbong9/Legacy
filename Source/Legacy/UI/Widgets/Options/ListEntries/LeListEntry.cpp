// bong9 All Rights Reserved

#include "LeListEntry.h"

#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"
#include "Legacy/UI/Widgets/Options/DataObjects/LeListDataObject.h"

void ULeListEntry::NativeOnListEntryWidgetHovered(bool bWasHovered)
{
	BP_OnListEntryWidgetHovered(bWasHovered, GetListItem() ? IsListItemSelected() : false);

	if (bWasHovered)
	{
		BP_OnToggleEntryWidgetHighlightState(true);
	}
	else
	{
		BP_OnToggleEntryWidgetHighlightState(GetListItem() && IsListItemExpanded());
	}
}

void ULeListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	
	OnOwningListDataObjectSet(CastChecked<ULeListDataObject>(ListItemObject));
}

void ULeListEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

	BP_OnToggleEntryWidgetHighlightState(bIsSelected);
}

void ULeListEntry::NativeOnEntryReleased()
{
	IUserObjectListEntry::NativeOnEntryReleased();

	NativeOnListEntryWidgetHovered(false);
}

FReply ULeListEntry::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();

	if (CommonInputSubsystem && CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		if (UWidget* WidgetToFocus = BP_GetWidgetToFocusForGamePad())
		{
			if (TSharedPtr<SWidget> SlateWidgetToFocus = WidgetToFocus->GetCachedWidget())
			{
				return FReply::Handled().SetUserFocus(SlateWidgetToFocus.ToSharedRef());
			}
		}
	}

	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void ULeListEntry::OnOwningListDataObjectSet(ULeListDataObject* InOwningListDataObject)
{
	if (SettingDisplayNameText)
	{
		SettingDisplayNameText->SetText(InOwningListDataObject->GetDataDisplayName());
	}

	if (!InOwningListDataObject->OnListDataModified.IsBoundToObject(this))
	{
		InOwningListDataObject->OnListDataModified.AddUObject(this, &ThisClass::OnOwningListDatObjectModified);
	}

	if (!InOwningListDataObject->OnDependentDataModified.IsBoundToObject(this))
	{
		InOwningListDataObject->OnDependentDataModified.AddUObject(this, &ThisClass::OnOwningDependencyDataObjectModified);
	}
	
	OnToggleEditableState(InOwningListDataObject->IsDataCurrentlyEditable());

	CachedOwningDataObject = InOwningListDataObject;
}

void ULeListEntry::OnOwningListDatObjectModified(ULeListDataObject* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
}

void ULeListEntry::OnOwningDependencyDataObjectModified(ULeListDataObject* OwningModifiedDependencyData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningDataObject)
	{
		OnToggleEditableState(CachedOwningDataObject->IsDataCurrentlyEditable());
	}
	
}

void ULeListEntry::OnToggleEditableState(bool bIsEditable)
{
	if (SettingDisplayNameText)
	{
		SettingDisplayNameText->SetIsEnabled(bIsEditable);
	}
}

void ULeListEntry::SelectThisEntryWidget()
{
	CastChecked<UListView>(GetOwningListView())->SetSelectedItem(GetListItem());
}
