// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_DataListEntryMapping.generated.h"

class ULeListEntry;
class ULeListDataObject;

/**
 * 
 */
UCLASS()
class LEGACY_API UDataAsset_DataListEntryMapping : public UDataAsset
{
	GENERATED_BODY()

public:
	TSubclassOf<ULeListEntry> FindEntryWidgetClassByDataObject(ULeListDataObject* InDataObject) const;

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<ULeListDataObject>, TSubclassOf<ULeListEntry>> DataObjectListEntryMap;
};
