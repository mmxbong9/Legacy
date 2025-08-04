// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "LeListDataObject.h"
#include "LeListDataObject_Collection.generated.h"

/**
 * 
 */
UCLASS()
class LEGACY_API ULeListDataObject_Collection : public ULeListDataObject
{
	GENERATED_BODY()

public:
	void AddChildListData(ULeListDataObject* InChildListData);
	
	//~ Begin ULeListDataObject Interface
	virtual TArray<ULeListDataObject*> GetAllChildListData() const override;
	virtual bool HasAnyChildListData() const override;
	//~ End ULeListDataObject Interface

private:
	UPROPERTY(Transient)
	TArray<ULeListDataObject*> ChildListDataArray;
};
