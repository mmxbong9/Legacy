// bong9 All Rights Reserved

#include "LeListView.h"

#include "Editor/WidgetCompilerLog.h"
#include "Legacy/UI/Widgets/Options/DataAsset_DataListEntryMapping.h"
#include "Legacy/UI/Widgets/Options/DataObjects/LeListDataObject.h"
#include "Legacy/UI/Widgets/Options/DataObjects/LeListDataObject_Collection.h"
#include "Legacy/UI/Widgets/Options/ListEntries/LeListEntry.h"


UUserWidget& ULeListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}

	if (TSubclassOf<ULeListEntry> FoundWidgetClass = DataListEntryMapping->FindEntryWidgetClassByDataObject(CastChecked<ULeListDataObject>(Item)))
	{
		return GenerateTypedEntry<ULeListEntry>(FoundWidgetClass, OwnerTable);
	}

	return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}

bool ULeListView::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	return !FirstSelectedItem->IsA<ULeListDataObject_Collection>();
}

#if WITH_EDITOR
void ULeListView::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!DataListEntryMapping)
	{
		CompileLog.Error(FText::FromString(
			TEXT("The variable DataListEntryMapping has no valid data asset assigned ") +
			GetClass()->GetName() +
			TEXT(" need a valid data asset to function properly")
			));
	}
}
#endif
