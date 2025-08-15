// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "LeActivatableWidget.generated.h"

class UCommonBoundActionBar;
class ALePlayerController;

/**
 * 
 */
UCLASS()
class LEGACY_API ULeActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetBackHandlerWithActionBarEnable(bool bEnable); 

protected:
	//~ Begin UUserWidget Interface 
	virtual void NativeConstruct() override;
	//~ End UUserWidget Interface

	void OnToggleMainMenuBoundActionTriggered();
	
	FUIActionBindingHandle ToggleMainMenuActionHandle;

	UPROPERTY(EditDefaultsOnly, Category="OptionsScreen", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ToggleMainMenuAction;

	UFUNCTION(BlueprintPure)
	ALePlayerController* GetOwningPlayerController();

	UPROPERTY(meta=(OptionalWidget, AllowPrivateAccess=true))
	TObjectPtr<UCommonBoundActionBar> CommonBoundActionBar;

private:
	TWeakObjectPtr<ALePlayerController> CachedOwningPC;
};
