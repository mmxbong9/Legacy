// bong9 All Rights Reserved

#include "LegacyFunctionLibrary.h"
#include "Legacy/Settings/LegacyDeveloperSettings.h"

TSoftClassPtr<ULeActivatableWidget> ULegacyFunctionLibrary::GetSoftWidgetClassByTag(UPARAM(meta=(Categories="Widget")) FGameplayTag InWidgetTag)
{
	const ULegacyDeveloperSettings* Settings = GetDefault<ULegacyDeveloperSettings>();
	
	checkf(Settings->WidgetClassMap.Contains(InWidgetTag), TEXT("Could not find corresponding widget under the tag %s"), *InWidgetTag.ToString());

	return Settings->WidgetClassMap.FindRef(InWidgetTag);
}

TSoftObjectPtr<UTexture2D> ULegacyFunctionLibrary::GetOptionsSoftImageByTag(FGameplayTag InImageTag)
{
	const ULegacyDeveloperSettings* Settings = GetDefault<ULegacyDeveloperSettings>();

	checkf(Settings->OptionsScreenSoftImageMap.Contains(InImageTag), TEXT("Could not find corresponding widget under the tag %s"), *InImageTag.ToString());

	return Settings->OptionsScreenSoftImageMap.FindRef(InImageTag);
}
