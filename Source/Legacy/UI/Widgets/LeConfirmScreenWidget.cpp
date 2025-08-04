// bong9 All Rights Reserved

#include "Legacy/UI/Widgets/LeConfirmScreenWidget.h"

#include "CommonTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "Components/LeCommonButtonBase.h"
#include "ICommonInputModule.h"

ULeConfirmInfoObject* ULeConfirmInfoObject::CreateOkScreen(const FText& InScreenTitle, const FText& InScreenMessage)
{
	ULeConfirmInfoObject* InfoObject = NewObject<ULeConfirmInfoObject>();
	InfoObject->ScreenTitle   = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmScreenButtonInfo OkButtonInfo;
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
	OkButtonInfo.ButtonTextToDisplay     = FText::FromString(TEXT("Ok"));

	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);
	
	return InfoObject;
}

ULeConfirmInfoObject* ULeConfirmInfoObject::CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMessage)
{
	ULeConfirmInfoObject* InfoObject = NewObject<ULeConfirmInfoObject>();
	InfoObject->ScreenTitle   = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmScreenButtonInfo YesButtonInfo;
	YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	YesButtonInfo.ButtonTextToDisplay     = FText::FromString(TEXT("Yes"));

	FConfirmScreenButtonInfo NoButtonInfo;
	NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	NoButtonInfo.ButtonTextToDisplay     = FText::FromString(TEXT("No"));

	InfoObject->AvailableScreenButtons.Add(YesButtonInfo);
	InfoObject->AvailableScreenButtons.Add(NoButtonInfo);

	return InfoObject;
}

ULeConfirmInfoObject* ULeConfirmInfoObject::CreateOkCancelScreen(const FText& InScreenTitle, const FText& InScreenMessage)
{
	ULeConfirmInfoObject* InfoObject = NewObject<ULeConfirmInfoObject>();
	InfoObject->ScreenTitle   = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmScreenButtonInfo OkButtonInfo;
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	OkButtonInfo.ButtonTextToDisplay     = FText::FromString(TEXT("Ok"));

	FConfirmScreenButtonInfo CancelButtonInfo;
	CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	CancelButtonInfo.ButtonTextToDisplay     = FText::FromString(TEXT("Cancel"));

	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);
	InfoObject->AvailableScreenButtons.Add(CancelButtonInfo);

	return InfoObject;
}

void ULeConfirmScreenWidget::InitConfirmScreen(ULeConfirmInfoObject* InScreenInfoObject, TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback)
{
	check(InScreenInfoObject && TitleText && MessageText && DynamicEntryBoxButtons);

	TitleText->SetText  (InScreenInfoObject->ScreenTitle);
	MessageText->SetText(InScreenInfoObject->ScreenMessage);

	// Checking if the entry box has old button created previoisly
	if (DynamicEntryBoxButtons->GetNumEntries() != 0)
	{
		// Clearing the old buttons the entry box has. The widget type for the entry box is specified in the child widget blueprint.
		DynamicEntryBoxButtons->Reset<ULeCommonButtonBase>(
			[](ULeCommonButtonBase& ExistingButton)
			{
				ExistingButton.OnClicked().Clear();
			}
		);
	}

	check(!InScreenInfoObject->AvailableScreenButtons.IsEmpty());

	for (const FConfirmScreenButtonInfo& AvailableButtonInfo : InScreenInfoObject->AvailableScreenButtons)
	{
		FDataTableRowHandle InputActionRowHandle;
		
		ULeCommonButtonBase* AddedButton = DynamicEntryBoxButtons->CreateEntry<ULeCommonButtonBase>();
		AddedButton->SetButtonText(AvailableButtonInfo.ButtonTextToDisplay);
		AddedButton->OnClicked().AddLambda(
			[ClickedButtonCallback, AvailableButtonInfo, this]()
			{
				ClickedButtonCallback(AvailableButtonInfo.ConfirmScreenButtonType);

				DeactivateWidget();
			}
		);
	}
}

UWidget* ULeConfirmScreenWidget::NativeGetDesiredFocusTarget() const
{
	if (DynamicEntryBoxButtons->GetNumEntries()	!= 0)
	{
		// set focus on the last button. So if there are two buttons, on is yes, one is no. Our gamepad will focus on the No Button
		DynamicEntryBoxButtons->GetAllEntries().Last()->SetFocus();
	}
	
	return Super::NativeGetDesiredFocusTarget();
}

