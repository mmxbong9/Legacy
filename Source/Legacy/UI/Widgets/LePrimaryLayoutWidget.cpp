// bong9 All Rights Reserved

#include "LePrimaryLayoutWidget.h"
#include "Legacy/Settings/LegacyGameplayTags.h"
#include "Legacy/Helpers/DebugHelper.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

UCommonActivatableWidgetContainerBase* ULePrimaryLayoutWidget::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
	checkf(RegisteredWidgetStackMap.Contains(InTag), TEXT("Can not find widget stack by the tag %s"), *InTag.ToString());

	return RegisteredWidgetStackMap.FindRef(InTag);
}

void ULePrimaryLayoutWidget::RegisterWidgetStack(UPARAM(meta=(Categories="WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, InStack);
		}
	}
}

void ULePrimaryLayoutWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterWidgetStack(LegacyGameplayTags::WidgetStack_MainMenu, MainMenuStack);
	RegisterWidgetStack(LegacyGameplayTags::WidgetStack_GameHud,  GameHudStack);
	RegisterWidgetStack(LegacyGameplayTags::WidgetStack_GameMenu, GameMenuStack);
	RegisterWidgetStack(LegacyGameplayTags::WidgetStack_Popup,    PopupStack);
}
