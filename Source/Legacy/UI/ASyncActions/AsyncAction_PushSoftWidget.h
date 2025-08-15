// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushSoftWidget.generated.h"

class ULeActivatableWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate, ULeActivatableWidget*, PushedWidget);

/**
 * 
 */
UCLASS()
class LEGACY_API UAsyncAction_PushSoftWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", HidePin="WorldContextObject", BlueprintInternalUseOnly=true), DisplayName="Push Soft Widget To Widget Stack")
	static UAsyncAction_PushSoftWidget* PushSoftWidget(const UObject* WorldContextObject,
		APlayerController* InOwningPlayerController, TSoftClassPtr<ULeActivatableWidget> InSoftWidgetClass,
		UPARAM(meta=(Categories="WidgetStack")) FGameplayTag InWidgetTag, bool bFocusOnNewlyPushedWidget = true, bool bUseBackKeyAndActionBar = true
		);

	//~ Begin UBlueprintAsyncActionBase Interface
	virtual void Activate() override;
	//~ End UBlueprintAsyncActionBase Interface

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate OnWidgetCreatedBeforePush;

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate AfterPush;

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	TWeakObjectPtr<APlayerController> CachedOwningPC;
	TSoftClassPtr<ULeActivatableWidget> CachedSoftWidgetClass;
	FGameplayTag CachedWidgetStackTag;
	bool bCachedFocusOnNewlyPushedWidget = false;
	bool bCachedUseBackKeyAndActionBar = true;
};
