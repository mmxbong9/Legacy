﻿// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "LegacyLoadingScreenSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig)
class LEGACY_API ULegacyLoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	TSubclassOf<UUserWidget> GetLoadingScreenWidgetClassChecked() const;
	
	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	TSoftClassPtr<UUserWidget> SoftLoadingScreenWidgetClass;

	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	float HoldLoadingScreenExtraSeconds = 3.f;

	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	bool bShouldLoadingScreenInEditor = false;
};
