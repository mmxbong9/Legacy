// bong9 All Rights Reserved

#include "LeListEntry_String.h"

#include "CommonInputSubsystem.h"
#include "Legacy/Helpers/DebugHelper.h"
#include "Legacy/UI/Widgets/Components/LeCommonButtonBase.h"
#include "Legacy/UI/Widgets/Components/LeCommonRotator.h"
#include "Legacy/UI/Widgets/Options/DataObjects/LeListDataObject_String.h"

void ULeListEntry_String::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PreviousOption->OnClicked().AddUObject(this, &ThisClass::OnPreviousOptionButtonClicked);
	NextOption->OnClicked().AddUObject(this, &ThisClass::OnNextOptionButtonClicked);
	
	AvailableOptions->OnClicked().AddLambda([this](){ SelectThisEntryWidget(); });
	AvailableOptions->OnRotatedEvent.AddUObject(this, &ThisClass::OnRotatorValueChanged);
}

void ULeListEntry_String::OnOwningListDataObjectSet(ULeListDataObject* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CastChecked<ULeListDataObject_String>(InOwningListDataObject);

	CachedOwningStringDataObject = CastChecked<ULeListDataObject_String>(InOwningListDataObject);

	AvailableOptions->PopulateTextLabels(CachedOwningStringDataObject->GetAvailableOptionsTextArray());
	AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
}

void ULeListEntry_String::OnOwningListDatObjectModified(ULeListDataObject* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	Super::OnOwningListDatObjectModified(OwningModifiedData, ModifyReason);

	if (CachedOwningStringDataObject)
	{
		AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	}
}

void ULeListEntry_String::OnToggleEditableState(bool bIsEditable)
{
	Super::OnToggleEditableState(bIsEditable);

	PreviousOption->SetIsEnabled(bIsEditable);
	AvailableOptions->SetIsEnabled(bIsEditable);
	NextOption->SetIsEnabled(bIsEditable);
}

void ULeListEntry_String::OnPreviousOptionButtonClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->BackToPreviousOption();
	}

	SelectThisEntryWidget();
}

void ULeListEntry_String::OnNextOptionButtonClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->AdvanceToNextOption();
	}

	SelectThisEntryWidget();
}

void ULeListEntry_String::OnRotatorValueChanged(int32 Value, bool bUserInitiated)
{
	if (!CachedOwningStringDataObject)
	{
		return;
	}

	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();

	if (!CommonInputSubsystem || !bUserInitiated)
	{
		return;
	}

	if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		CachedOwningStringDataObject->OnRotatorInitiatedValueChange(AvailableOptions->GetSelectedText());
	}
}
