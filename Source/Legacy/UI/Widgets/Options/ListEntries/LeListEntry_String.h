// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "LeListEntry.h"
#include "LeListEntry_String.generated.h"

class ULeListDataObject_String;
class ULeCommonRotator;
class ULeCommonButtonBase;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class LEGACY_API ULeListEntry_String : public ULeListEntry
{
	GENERATED_BODY()

protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface
	
	//~ Begin ULeListEntry Interface
	virtual void OnOwningListDataObjectSet(ULeListDataObject* InOwningListDataObject) override;
	virtual void OnOwningListDatObjectModified(ULeListDataObject* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;
	virtual void OnToggleEditableState(bool bIsEditable) override;
	//~ End ULeListEntry Interface
	
private:
	void OnPreviousOptionButtonClicked();
	void OnNextOptionButtonClicked();

	void OnRotatorValueChanged(int32 Value, bool bUserInitiated);
	
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true))
	TObjectPtr<ULeCommonButtonBase> PreviousOption;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true))
	TObjectPtr<ULeCommonRotator> AvailableOptions;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true))
	TObjectPtr<ULeCommonButtonBase> NextOption;
	//***** Bound Widgets *****//

	UPROPERTY(Transient)
	TObjectPtr<ULeListDataObject_String> CachedOwningStringDataObject;
};
