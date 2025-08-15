// bong9 All Rights Reserved

#include "LeActivatableWidget.h"

#include "ICommonInputModule.h"
#include "Input/CommonBoundActionBar.h"
#include "Input/CommonUIInputTypes.h"
#include "Legacy/Legacy.h"
#include "Legacy/Character/LeCharacter.h"
#include "Legacy/Player/LePlayerController.h"

ALePlayerController* ULeActivatableWidget::GetOwningPlayerController()
{
	if (!CachedOwningPC.IsValid())
	{
		CachedOwningPC = GetOwningPlayer<ALePlayerController>();
	}

	return CachedOwningPC.IsValid() ? CachedOwningPC.Get() : nullptr;
}

void ULeActivatableWidget::SetBackHandlerWithActionBarEnable(bool bEnable)
{
	if (IsValid(CommonBoundActionBar.Get()))
	{
		bIsBackHandler = bEnable;
		CommonBoundActionBar->SetVisibility(bEnable ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	
	LOG_INFO("ActionBarEnable: %s", bEnable ? TEXT("true") : TEXT("false"));
}

void ULeActivatableWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ToggleMainMenuAction.IsNull())
	{
		ToggleMainMenuActionHandle = RegisterUIActionBinding(
		FBindUIActionArgs(
			ToggleMainMenuAction,
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnToggleMainMenuBoundActionTriggered)
			)
		);
	}
}

void ULeActivatableWidget::OnToggleMainMenuBoundActionTriggered()
{
	UE_LOG(LogTemp, Warning, TEXT("Toggle Main Menu Bound Action Triggered"));

	ALePlayerController* PC = Cast<ALePlayerController>(GetOwningPlayerController());
	
	if (PC->GetPawn()->IsA(ALeCharacter::StaticClass()))
	{
		PC->ToggleMainMenu();
	}
}
