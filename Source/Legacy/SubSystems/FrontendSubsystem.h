// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Legacy/Types/LeFrontendTypes.h"
#include "Legacy/UI/Widgets/LePrimaryLayoutWidget.h"
#include "FrontendSubsystem.generated.h"

class ULeCommonButtonBase;
class ULeActivatableWidget;
class ULePrimaryLayoutWidget;
struct FGameplayTag;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionTextUpdatedDelegate, ULeCommonButtonBase*, BroadcastingButton, FText, DescriptionText);

UCLASS()
class LEGACY_API UFrontendSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UFrontendSubsystem* Get(const UObject* InWorldContextObject);

	FORCEINLINE ULePrimaryLayoutWidget* GetCreatedPrimaryLayout()  const { return CreatedPrimaryLayout.Get(); }

	//~ Begin USubsystem Interface
	virtual bool ShouldCreateSubsystem(UObject* InOuter) const override;
	//~ End USubsystem Interface

	UFUNCTION(BlueprintCallable)
	void UnregisterPrimaryLayoutWidget();
	
	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(ULePrimaryLayoutWidget* InCreatedWidget);

	void PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag, TSoftClassPtr<ULeActivatableWidget> InSoftWidgetClass,
		TFunction<void(EAsyncPushWidgetState, ULeActivatableWidget*)> ASyncPushStateCallback);

	void PushConfirmScreenToPopupStackAsync(EConfirmScreenType InScreenType, const FText& InScreenTitle, const FText& InScreenMessage,
		TFunction<void(EConfirmScreenButtonType)> ButtonClickedCallback);

	UFUNCTION(BlueprintCallable)
	void RemoveActivatedPopup();

	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdatedDelegate OnButtonDescriptionTextUpdated;

private:
	UPROPERTY(Transient) TObjectPtr<ULePrimaryLayoutWidget> CreatedPrimaryLayout;
};