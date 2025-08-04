// bong9 All Rights Reserved

#include "LeListDataObject_Collection.h"

void ULeListDataObject_Collection::AddChildListData(ULeListDataObject* InChildListData)
{
	// Notify the child list data to init itself
	InChildListData->InitDataObject();

	// Set the child list data's parent to this
	InChildListData->SetParentData(this);
	
	ChildListDataArray.Add(InChildListData);
}

TArray<ULeListDataObject*> ULeListDataObject_Collection::GetAllChildListData() const
{
	return ChildListDataArray;
}

bool ULeListDataObject_Collection::HasAnyChildListData() const
{
	return !ChildListDataArray.IsEmpty();
}
