// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "LeListEntry.h"
#include "LeListEntry_KeyRemap.generated.h"

class ULeListDataObject_KeyRemap;
class ULeCommonButtonBase;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class LEGACY_API ULeListEntry_KeyRemap : public ULeListEntry
{
	GENERATED_BODY()

protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface
	
	//~ Begin ULeListEntry Interface
	virtual void OnOwningListDataObjectSet(ULeListDataObject* InOwningListDataObject) override;
	virtual void OnOwningListDatObjectModified(ULeListDataObject* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;
	//~ End ULeListEntry Interface
	
private:
	void OnRemapKeyButtonClicked();
	void OnResetKeyBindingButtonClicked();

	void OnKeyToRemapPressed(const FKey& PressedKey);
	void OnKeyToRemapCanceled(const FString& CanceledReason);
	
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true))
	TObjectPtr<ULeCommonButtonBase> RemapKeyButton;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true))
	TObjectPtr<ULeCommonButtonBase> ResetKeyBindingButton;
	//***** Bound Widgets *****//

	UPROPERTY(Transient)
	TObjectPtr<ULeListDataObject_KeyRemap> CachedOwningKeyRemapDataObject;
};
