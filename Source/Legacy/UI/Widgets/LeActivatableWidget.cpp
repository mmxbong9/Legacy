// bong9 All Rights Reserved

#include "LeActivatableWidget.h"

#include "Legacy/Player/LePlayerController.h"

ALePlayerController* ULeActivatableWidget::GetOwningPlayerController()
{
	if (!CachedOwningPC.IsValid())
	{
		CachedOwningPC = GetOwningPlayer<ALePlayerController>();
	}

	return CachedOwningPC.IsValid() ? CachedOwningPC.Get() : nullptr;
}
