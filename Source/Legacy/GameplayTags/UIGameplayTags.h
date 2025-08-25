#pragma once

#include "NativeGameplayTags.h"

/**
 * @namespace UIGameplayTags
 * @brief UE5 권장 네이티브 GameplayTag 선언/정의 패턴을 사용
 *        UI 관련 Tag 들을 선언/관리
 * 
 * - UE_DECLARE_GAMEPLAY_TAG_EXTERN : 헤더에 태그 심볼 선언
 * - UE_DEFINE_GAMEPLAY_TAG         : CPP에서 태그 정의 및 자동 등록
 */
namespace UIGameplayTags
{
	// Widget Stacks
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Modal);    // Modal
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_GameMenu); // Pause, Inventory
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_GameHud);  // Health bar, Player Status 
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_MainMenu); // MainMenu

	// Widgets
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Widget_PressAnyKeyScreen);
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Widget_MainMenuScreen);
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Widget_OptionsScreen);
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Widget_ConfirmScreen);
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Widget_KeyRemapScreen);

	// Options Image
	LEGACY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Image_TestImage);
}