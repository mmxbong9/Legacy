// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "Legacy/UI/Widgets/LeActivatableWidget.h"
#include "LegacyDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Legacy UI Settings"))
class LEGACY_API ULegacyDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category="Widget Reference", meta=(ForceInlineRow, Categories="Widget"))
	TMap<FGameplayTag, TSoftClassPtr<ULeActivatableWidget>> WidgetClassMap;

	UPROPERTY(Config, EditAnywhere, Category="Options Image Reference", meta=(ForceInlineRow, Categories="Image"))
	TMap<FGameplayTag, TSoftObjectPtr<UTexture2D>> OptionsScreenSoftImageMap;
};
