// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Legacy/Types/LeFrontendTypes.h"
#include "Legacy/UI/Widgets/LeActivatableWidget.h"
#include "LeOptionScreenWidget.generated.h"

class ULeListDataObject;
class UOptionsDetailsView;
class ULeTabListWidget;
class ULeOptionsDataRegistry;
class ULeListView;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class LEGACY_API ULeOptionScreenWidget : public ULeActivatableWidget
{
	GENERATED_BODY()

protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

	//~ Begin UCommonActivatableWidget Interface
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface
	
private:
	ULeOptionsDataRegistry* GetOrCreateDataRegistry();
	
	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	UFUNCTION()
	void OnOptionsTabSelected(FName TabId);

	void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
	void OnListViewItemSelected(UObject* InSelectedItem);

	FString TryGetEntryWidgetClassName(UObject* InOwningListItem) const;

	void OnListViewListDataModified(ULeListDataObject* ModifiedData, EOptionsListDataModifyReason ModifyReason);

	//***** Bound Widgets *****//
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<ULeTabListWidget> TabListWidget_OptionsTabs;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<ULeListView> CommonListView_OptionsList;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOptionsDetailsView> DetailsView_ListEntryInfo;
	//***** Bound Widgets *****//

	// Handle the creation of data in the options screen, Direct access to this variable is forbidden.
	UPROPERTY(Transient)
	TObjectPtr<ULeOptionsDataRegistry> CreatedOwningDataRegistry;
	
	UPROPERTY(EditDefaultsOnly, Category="OptionsScreen", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;

	UPROPERTY(Transient)
	TArray<ULeListDataObject*> ResettableDataArray;

	bool bIsResettingData = false;
};
