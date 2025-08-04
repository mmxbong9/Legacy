// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Legacy/Types/LeFrontendStructs.h"
#include "Legacy/Types/LeFrontendTypes.h"
#include "UObject/NoExportTypes.h"
#include "LeListDataObject.generated.h"

#define LIST_DATA_ACCESSOR(DataType,PropertyName) \
	FORCEINLINE DataType Get##PropertyName() const { return PropertyName; } \
	void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName; }

/**
 * 
 */
UCLASS(Abstract)
class LEGACY_API ULeListDataObject : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModifiedDelegate, ULeListDataObject*, EOptionsListDataModifyReason)
	FOnListDataModifiedDelegate OnListDataModified;
	FOnListDataModifiedDelegate OnDependentDataModified;
	
	LIST_DATA_ACCESSOR(FName,DataID)
	LIST_DATA_ACCESSOR(FText,DataDisplayName)
	LIST_DATA_ACCESSOR(FText,DescriptionRichText)
	LIST_DATA_ACCESSOR(FText,DisabledRichText)
	LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>,SoftDescriptionImage)
	LIST_DATA_ACCESSOR(TObjectPtr<ULeListDataObject>,ParentData)

	void InitDataObject();

	// Empty in the base class. Child class ListDataObject_Collection should override it. The function should return all the child data a tab has
	virtual TArray<ULeListDataObject*> GetAllChildListData() const { return TArray<ULeListDataObject*>(); }
	virtual bool HasAnyChildListData() const { return false; }

	void SetShouldApplySettingsImmediately(bool bShouldApplyRightAway) { bShouldApplyChangeImmediatly = bShouldApplyRightAway; }

	// The child class should override them to provide implementations for resetting the data
	virtual bool HasDefaultValue() const { return false; }
	virtual bool CanResetBackToDefaultValue() const { return false; }
	virtual bool TryResetBackToDefaultValue() { return false; }

	// Gets called from OptionsDataRegister for adding in edit conditions for the constructed list data objects
	void AddEditCondition(const FOptionsDataEditConditionDescriptor& InEditCondition);

	// Gets called from OptionsDataRegistry to add in dependency data
	void AddEditDependencyData(ULeListDataObject* InDependencyData);

	bool IsDataCurrentlyEditable();

protected:
	// Empty in th base class. The child classes should override it to handle the initialized needed accrodingly
	virtual void OnDataObjectInitialized();

	virtual void NotifyListDataModified(ULeListDataObject* ModifiedData, EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified);

	// The child class should override this to allow the value be set to the forced string value
	virtual bool CanSetToForcedStringValue(const FString& InForcedValue) const { return false; }

	// The child class should override this to specify how to set the current value to the forced value
	virtual void OnSetToForcedStringValue(const FString& InForcedValue) {}

	// This function will be called when the value of the dependency data chs changed, The child class can override this function to handle the custom logic needed. Super call is expected.
	virtual void OnEditDependencyDataModified(ULeListDataObject* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason);
	
private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;

	UPROPERTY(Transient)
	TObjectPtr<ULeListDataObject> ParentData;

	bool bShouldApplyChangeImmediatly = false;

	UPROPERTY(Transient)
	TArray<FOptionsDataEditConditionDescriptor> EditConditionDescArray;
};
