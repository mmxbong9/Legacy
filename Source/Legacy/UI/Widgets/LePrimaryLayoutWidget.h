// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "LePrimaryLayoutWidget.generated.h"

class UCommonActivatableWidgetContainerBase;
class UCommonActivatableWidgetStack;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class LEGACY_API ULePrimaryLayoutWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(const FGameplayTag& InTag) const;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category=WidgetStacks) TObjectPtr<UCommonActivatableWidgetStack> MainMenuStack;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category=WidgetStacks) TObjectPtr<UCommonActivatableWidgetStack> GameHudStack;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category=WidgetStacks) TObjectPtr<UCommonActivatableWidgetStack> GameMenuStack;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category=WidgetStacks) TObjectPtr<UCommonActivatableWidgetStack> PopupStack;

protected:
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetStack(UPARAM(meta=(Categories="WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack);

	//- UUserWidget
	virtual void NativeOnInitialized() override;
	//- UUserWidget
	
private:
	TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> RegisteredWidgetStackMap;
};
