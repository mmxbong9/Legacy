// bong9 All Rights Reserved

#include "LegacyGameplayTags.h"

namespace LegacyGameplayTags
{
	// widget stack
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Popup,    "WidgetStack.Popup");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameMenu, "WidgetStack.GameMenu");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameHud,  "WidgetStack.GameHud");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_MainMenu, "WidgetStack.MainMenu");

	// widgets
	UE_DEFINE_GAMEPLAY_TAG(Widget_PressAnyKeyScreen, "Widget.PressAnyKeyScreen");
	UE_DEFINE_GAMEPLAY_TAG(Widget_MainMenuScreen,    "Widget.MainMenuScreen");
	UE_DEFINE_GAMEPLAY_TAG(Widget_OptionsScreen,     "Widget.OptionsScreen");

	UE_DEFINE_GAMEPLAY_TAG(Widget_ConfirmScreen,     "Widget.ConfirmScreen");
	
	UE_DEFINE_GAMEPLAY_TAG(Widget_KeyRemapScreen,    "Widget.KeyRemapScreen");

	// Options Image
	UE_DEFINE_GAMEPLAY_TAG(Image_TestImage, "Image.TestImage");
}