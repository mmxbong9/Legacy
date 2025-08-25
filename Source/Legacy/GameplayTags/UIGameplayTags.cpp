#include "UIGameplayTags.h"

namespace UIGameplayTags
{
	// Widget Stacks
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Modal,    "WidgetStack.Modal");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameMenu, "WidgetStack.GameMenu");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameHud,  "WidgetStack.GameHud");
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_MainMenu, "WidgetStack.MainMenu");

	// Widgets
	UE_DEFINE_GAMEPLAY_TAG(Widget_PressAnyKeyScreen, "Widget.PressAnyKeyScreen");
	UE_DEFINE_GAMEPLAY_TAG(Widget_MainMenuScreen,    "Widget.MainMenuScreen");
	UE_DEFINE_GAMEPLAY_TAG(Widget_OptionsScreen,     "Widget.OptionsScreen");
	UE_DEFINE_GAMEPLAY_TAG(Widget_ConfirmScreen,     "Widget.ConfirmScreen");
	UE_DEFINE_GAMEPLAY_TAG(Widget_KeyRemapScreen,    "Widget.KeyRemapScreen");

	// Options Image
	UE_DEFINE_GAMEPLAY_TAG(Image_TestImage, "Image.TestImage");
}