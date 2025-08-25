// bong9 All Rights Reserved

#include "LePrimaryLayoutWidget.h"
#include "Legacy/GameplayTags/UIGameplayTags.h"
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

	RegisterWidgetStack(UIGameplayTags::WidgetStack_MainMenu, MainMenuStack);
	RegisterWidgetStack(UIGameplayTags::WidgetStack_GameHud,  GameHudStack);
	RegisterWidgetStack(UIGameplayTags::WidgetStack_GameMenu, GameMenuStack);
	RegisterWidgetStack(UIGameplayTags::WidgetStack_Modal,    ModalStack);
}
