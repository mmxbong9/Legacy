// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Legacy/Types/LeFrontendTypes.h"
#include "LeListEntry.generated.h"

class ULeListDataObject;
class UCommonTextBlock;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class LEGACY_API ULeListEntry : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On List Entry Widget Hovered"))
	void BP_OnListEntryWidgetHovered(bool bWasHovered, bool bIsEntryWidgetStillSelected);
	void NativeOnListEntryWidgetHovered(bool bWasHovered);

protected:
	// The child widget blueprint should override this function for the gamepad interaction to function properly
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Get Widget To Focus For Gamepad"))
	UWidget* BP_GetWidgetToFocusForGamePad() const;

	// The child blueprint should override it to handle the highlight state when this entry widget is hovered or selected
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On Toggle Entry Widget Highlight State"))
	void BP_OnToggleEntryWidgetHighlightState(bool bShouldHighlight) const;
	
	//~ Begin IUserObjectListEntry Interface
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual void NativeOnEntryReleased() override;
	//~ End IUserObjectListEntry Interface

	//~ Begin UUserWidget Interface
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	//~ End UUserWidget Interface

	// The child class should override this function to handle the initialization needed. Super call is expected
	virtual void OnOwningListDataObjectSet(ULeListDataObject* InOwningListDataObject);

	// The child class should override this function to update the UI values after the data object has been modified. Super call is not needed
	virtual void OnOwningListDatObjectModified(ULeListDataObject* OwningModifiedData, EOptionsListDataModifyReason ModifyReason);

	virtual void OnOwningDependencyDataObjectModified(ULeListDataObject* OwningModifiedDependencyData, EOptionsListDataModifyReason ModifyReason);

	// The child class should override this to change editable stat of the widgets it owns. Super call is expected.
	virtual void OnToggleEditableState(bool bIsEditable);
	
	void SelectThisEntryWidget();

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UCommonTextBlock> SettingDisplayNameText;
	//***** Bound Widgets *****//

	UPROPERTY(Transient)
	ULeListDataObject* CachedOwningDataObject;
};
