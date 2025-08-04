// bong9 All Rights Reserved

#include "OptionsDetailsView.h"

#include "CommonTextBlock.h"
#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "DataObjects/LeListDataObject.h"

void UOptionsDetailsView::UpdateDetailsViewInfo(ULeListDataObject* InDataObject, const FString& InEntryWidgetClassName)
{
	if (!InDataObject)
	{
		return;
	}

	TitleText->SetText(InDataObject->GetDataDisplayName());

	if (!InDataObject->GetSoftDescriptionImage().IsNull())
	{
		DescriptionLazyImage->SetBrushFromLazyTexture(InDataObject->GetSoftDescriptionImage());
		DescriptionLazyImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		DescriptionLazyImage->SetVisibility(ESlateVisibility::Collapsed);
	}

	DescriptionRichText->SetText(InDataObject->GetDescriptionRichText());

	const FString DynamicDetails = FString::Printf(
		TEXT("Data Object Class: <Bold>%s</>\n\nEntry Widget Class: <Bold>%s</>"),
		*InDataObject->GetClass()->GetName(),
		*InEntryWidgetClassName
	);
		
	DynamicRichText->SetText(FText::FromString(DynamicDetails));
	
	DisableReasonRichText->SetText(InDataObject->IsDataCurrentlyEditable() ? FText::GetEmpty() : InDataObject->GetDisabledRichText());
}

void UOptionsDetailsView::ClearDetailsViewInfo()
{
	TitleText->SetText(FText::GetEmpty());
	DescriptionLazyImage->SetVisibility(ESlateVisibility::Collapsed);
	DescriptionRichText->SetText(FText::GetEmpty());
	DynamicRichText->SetText(FText::GetEmpty());
	DisableReasonRichText->SetText(FText::GetEmpty());
}

void UOptionsDetailsView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ClearDetailsViewInfo();
}
