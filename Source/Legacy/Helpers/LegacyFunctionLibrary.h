// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "LegacyFunctionLibrary.generated.h"

class ULeActivatableWidget;

/**
 * 
 */
UCLASS()
class LEGACY_API ULegacyFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category="Legacy Function Library")
	static TSoftClassPtr<ULeActivatableWidget> GetSoftWidgetClassByTag(UPARAM(meta=(Categories="Widget")) FGameplayTag InWidgetTag);

	UFUNCTION(BlueprintPure, Category="Legacy Function Library")
	static TSoftObjectPtr<UTexture2D> GetOptionsSoftImageByTag(UPARAM(meta=(Categories="Image")) FGameplayTag InImageTag);
};