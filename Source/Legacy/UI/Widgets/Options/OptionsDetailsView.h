// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsDetailsView.generated.h"

class ULeListDataObject;
class UCommonTextBlock;
class UCommonLazyImage;
class UCommonRichTextBlock;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class LEGACY_API UOptionsDetailsView : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateDetailsViewInfo(ULeListDataObject* InDataObject, const FString& InEntryWidgetClassName = FString());
	void ClearDetailsViewInfo();

protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

private:
	//***** Bound Widgets *****/
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> TitleText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonLazyImage> DescriptionLazyImage;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonRichTextBlock> DescriptionRichText;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonRichTextBlock> DynamicRichText;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonRichTextBlock> DisableReasonRichText;
	//***** Bound Widgets *****/
};
