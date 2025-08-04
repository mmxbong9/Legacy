// bong9 All Rights Reserved

#include "DataAsset_DataListEntryMapping.h"

#include "DataObjects/LeListDataObject.h"

TSubclassOf<ULeListEntry> UDataAsset_DataListEntryMapping::FindEntryWidgetClassByDataObject(ULeListDataObject* InDataObject) const
{
	check(InDataObject);

	for (UClass* DataObjectClass = InDataObject->GetClass(); DataObjectClass; DataObjectClass = DataObjectClass->GetSuperClass())
	{
		if (TSubclassOf<ULeListDataObject> ConvertedDataObjectClass = TSubclassOf<ULeListDataObject>(DataObjectClass))
		{
			if (DataObjectListEntryMap.Contains(ConvertedDataObjectClass))
			{
				return DataObjectListEntryMap.FindRef(ConvertedDataObjectClass);
			}
		}
	}

	return TSubclassOf<ULeListEntry>();
}
