// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "LeActivatableWidget.generated.h"

class ALePlayerController;

/**
 * 
 */
UCLASS()
class LEGACY_API ULeActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure)
	ALePlayerController* GetOwningPlayerController();

private:
	TWeakObjectPtr<ALePlayerController> CachedOwningPC;
};
