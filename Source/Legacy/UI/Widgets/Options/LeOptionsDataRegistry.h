// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LeOptionsDataRegistry.generated.h"

class ULeListDataObject;
class ULeListDataObject_Collection;

/**
 * 
 */
UCLASS()
class LEGACY_API ULeOptionsDataRegistry : public UObject
{
	GENERATED_BODY()

public:
	// Gets called by options screen right after the object of thpe ULeOptionsDataRegistry is created 
	void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);

	const TArray<ULeListDataObject_Collection*>& GetRegisteredOptionsTabCollections() const { return RegisteredOptionsTabCollections; }

	TArray<ULeListDataObject*> GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const;

private:
	void FindChildListDataRecursively(ULeListDataObject* InParentData, TArray<ULeListDataObject*>& OutChildrenListData) const;
	
	void InitGameplayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControlCollectionTab(ULocalPlayer* InOwningLocalPlayer);

	UPROPERTY(Transient)
	TArray<ULeListDataObject_Collection*> RegisteredOptionsTabCollections;
};
