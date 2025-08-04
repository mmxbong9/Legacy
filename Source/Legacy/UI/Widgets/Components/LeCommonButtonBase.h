// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "LeCommonButtonBase.generated.h"

class UCommonLazyImage;
class UCommonTextBlock;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class LEGACY_API ULeCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText);

	UFUNCTION(BlueprintCallable)
	FText GetButtonDisplayText() const;

	UFUNCTION(BlueprintCallable)
	void SetButtonDisplayImage(const FSlateBrush& InBrush);
	
private:
	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface

	//~ Begin UCommonButtonBase Interface
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	//~ End UCommonButtonBase Interface

	//***** Bound Widgets *****/
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> ButtonText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess=true))
	TObjectPtr<UCommonLazyImage> ButtonLazyImage;
	//***** Bound Widgets *****/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Legacy Button", meta=(AllowPrivateAccess=true))
	FText DisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Legacy Button", meta=(AllowPrivateAccess=true))
	bool bUserUpperCaseForButtonText = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Legacy Button", meta=(AllowPrivateAccess=true))
	FText DescriptionText;
};
