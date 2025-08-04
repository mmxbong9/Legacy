// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Legacy/UI/Widgets/LeActivatableWidget.h"
#include "Legacy/Types/LeFrontendTypes.h"
#include "LeConfirmScreenWidget.generated.h"

class UDynamicEntryBox;
class UCommonTextBlock;

USTRUCT(BlueprintType)
struct FConfirmScreenButtonInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmScreenButtonType ConfirmScreenButtonType = EConfirmScreenButtonType::Max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonTextToDisplay;
};

UCLASS()
class LEGACY_API ULeConfirmInfoObject : public UObject
{
	GENERATED_BODY()

public:
	static ULeConfirmInfoObject* CreateOkScreen      (const FText& InScreenTitle, const FText& InScreenMessage);
	static ULeConfirmInfoObject* CreateYesNoScreen   (const FText& InScreenTitle, const FText& InScreenMessage);
	static ULeConfirmInfoObject* CreateOkCancelScreen(const FText& InScreenTitle, const FText& InScreenMessage);
	
	UPROPERTY(Transient)
	FText ScreenTitle;
	
	UPROPERTY(Transient)
	FText ScreenMessage;
	
	UPROPERTY(Transient)
	TArray<FConfirmScreenButtonInfo> AvailableScreenButtons;
};

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class LEGACY_API ULeConfirmScreenWidget : public ULeActivatableWidget
{
	GENERATED_BODY()

public:
	// Gets called outside of the class when this widget is constructed and before it's pushed to the modal(popup) stack
	void InitConfirmScreen(ULeConfirmInfoObject* InScreenInfoObject, TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback);

protected:
	//~ Begin UCommonActivatableWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> TitleText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> MessageText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UDynamicEntryBox> DynamicEntryBoxButtons;
};
