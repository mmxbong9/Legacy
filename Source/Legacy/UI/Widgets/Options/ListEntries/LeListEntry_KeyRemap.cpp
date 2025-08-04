// bong9 All Rights Reserved

#include "LeListEntry_KeyRemap.h"

#include "Legacy/Settings/LegacyGameplayTags.h"
#include "Legacy/Helpers/DebugHelper.h"
#include "Legacy/Helpers/LegacyFunctionLibrary.h"
#include "Legacy/SubSystems/FrontendSubsystem.h"
#include "Legacy/UI/Widgets/Components/LeCommonButtonBase.h"
#include "Legacy/UI/Widgets/Options/KeyRemapScreenWidget.h"
#include "Legacy/UI/Widgets/Options/DataObjects/LeListDataObject_KeyRemap.h"

void ULeListEntry_KeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RemapKeyButton->OnClicked().AddUObject(this, &ThisClass::OnRemapKeyButtonClicked);
	ResetKeyBindingButton->OnClicked().AddUObject(this, &ThisClass::OnResetKeyBindingButtonClicked);
}

void ULeListEntry_KeyRemap::OnOwningListDataObjectSet(ULeListDataObject* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningKeyRemapDataObject = CastChecked<ULeListDataObject_KeyRemap>(InOwningListDataObject);

	RemapKeyButton->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
}

void ULeListEntry_KeyRemap::OnOwningListDatObjectModified(ULeListDataObject* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningKeyRemapDataObject)
	{
		RemapKeyButton->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
	}
	
	Super::OnOwningListDatObjectModified(OwningModifiedData, ModifyReason);
}

void ULeListEntry_KeyRemap::OnRemapKeyButtonClicked()
{
	SelectThisEntryWidget();
	
	UFrontendSubsystem::Get(this)->PushSoftWidgetToStackAsync(
		LegacyGameplayTags::WidgetStack_Popup,
		ULegacyFunctionLibrary::GetSoftWidgetClassByTag(LegacyGameplayTags::Widget_KeyRemapScreen),
		[this](EAsyncPushWidgetState PushState, ULeActivatableWidget* PushedWidget)
		{
			if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UKeyRemapScreenWidget* CreatedKeyRemapScreen = CastChecked<UKeyRemapScreenWidget>(PushedWidget);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &ThisClass::OnKeyToRemapPressed);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeySelectCanceled.BindUObject(this, &ThisClass::OnKeyToRemapCanceled);
				
				if (CachedOwningKeyRemapDataObject)
				{
					CreatedKeyRemapScreen->SetDesiredInputTypeToFilter(CachedOwningKeyRemapDataObject->GetDesiredInputKeyType());
				}
			}
		}
	);
}

void ULeListEntry_KeyRemap::OnResetKeyBindingButtonClicked()
{
	SelectThisEntryWidget();

	if (!CachedOwningKeyRemapDataObject)
	{
		return;
	}
	
	// Check if the current key is already the default key. Display an OK screen that says this is already the default to player
	if (!CachedOwningKeyRemapDataObject->CanResetBackToDefaultValue())
	{
		UFrontendSubsystem::Get(this)->PushConfirmScreenToPopupStackAsync(
			EConfirmScreenType::Ok,
			FText::FromString(TEXT("Reset Key Mapping")),
			FText::FromString(TEXT("The key binding for ") + CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT(" is already set to default")),
			[](EConfirmScreenButtonType ClickedButton){}
		);

		return;
	}
	
	// Reset the key binding back to default
	UFrontendSubsystem::Get(this)->PushConfirmScreenToPopupStackAsync(
		EConfirmScreenType::YesNo,
		FText::FromString(TEXT("Reset Key Mapping")),
		FText::FromString(TEXT("Are you sure you want to key binding for ") + CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT(" ?")),
		[this](EConfirmScreenButtonType ClickedButton)
		{
			if (ClickedButton == EConfirmScreenButtonType::Confirmed)
			{
				CachedOwningKeyRemapDataObject->TryResetBackToDefaultValue();
			}
		}
	);
}

void ULeListEntry_KeyRemap::OnKeyToRemapPressed(const FKey& PressedKey)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CachedOwningKeyRemapDataObject->BindNewInputKey(PressedKey);
	}
}

void ULeListEntry_KeyRemap::OnKeyToRemapCanceled(const FString& CanceledReason)
{
	UFrontendSubsystem::Get(this)->PushConfirmScreenToPopupStackAsync(
		EConfirmScreenType::Ok,
		FText::FromString(TEXT("key Remap")),
		FText::FromString(CanceledReason),
		[](EConfirmScreenButtonType ClickedButton){}
	);
}
