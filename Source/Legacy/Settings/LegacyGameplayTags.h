// bong9 All Rights Reserved

#pragma once

#include "NativeGameplayTags.h"

namespace LegacyGameplayTags
{
	// widget stack
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Popup);    // Popup
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_GameMenu); // Pause, Inventory
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_GameHud);  // Health bar, Player Status 
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_MainMenu); // MainMenu

	// widgets
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Widget_PressAnyKeyScreen);
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Widget_MainMenuScreen);
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Widget_OptionsScreen);
	
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Widget_ConfirmScreen);

	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Widget_KeyRemapScreen);

	// Options Image
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Image_TestImage);
}