// bong9 All Rights Reserved

#include "LeCommonButtonBase.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "Legacy/SubSystems/FrontendSubsystem.h"

void ULeCommonButtonBase::SetButtonText(FText InText)
{
	if (ButtonText && !InText.IsEmpty())
	{
		ButtonText->SetText(bUserUpperCaseForButtonText ? InText.ToUpper() : InText);
	}
}

FText ULeCommonButtonBase::GetButtonDisplayText() const
{
	if (ButtonText)
	{
		return ButtonText->GetText();
	}
	
	return FText();
}

void ULeCommonButtonBase::SetButtonDisplayImage(const FSlateBrush& InBrush)
{
	if (ButtonLazyImage)
	{
		ButtonLazyImage->SetBrush(InBrush);
	}
}

void ULeCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(DisplayText);
}

void ULeCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (ButtonText && GetCurrentTextStyleClass())
	{
		ButtonText->SetStyle(GetCurrentTextStyleClass());
	}
}

void ULeCommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (!DescriptionText.IsEmpty())
	{
		UFrontendSubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, DescriptionText);
	}
}

void ULeCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	
	UFrontendSubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, FText::GetEmpty());
}
