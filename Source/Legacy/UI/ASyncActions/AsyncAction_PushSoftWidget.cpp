// bong9 All Rights Reserved

#include "AsyncAction_PushSoftWidget.h"
#include "Legacy/Subsystems/FrontendSubsystem.h"
#include "Legacy/UI/Widgets/LeActivatableWidget.h"

UAsyncAction_PushSoftWidget* UAsyncAction_PushSoftWidget::PushSoftWidget(const UObject* WorldContextObject,
                                                                         APlayerController* InOwningPlayerController, TSoftClassPtr<ULeActivatableWidget> InSoftWidgetClass,
                                                                         UPARAM(meta=(Categories="WidgetStack")) FGameplayTag InWidgetTag, bool bFocusOnNewlyPushedWidget)
{
	checkf(!InSoftWidgetClass.IsNull(), TEXT("PushSoftWidgetToStack was passed a null soft widget class"));

	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushSoftWidget* Node = NewObject<UAsyncAction_PushSoftWidget>();
			Node->CachedOwningWorld               = World;
			Node->CachedOwningPC                  = InOwningPlayerController;
			Node->CachedSoftWidgetClass           = InSoftWidgetClass;;
			Node->CachedWidgetStackTag            = InWidgetTag;
			Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;
			Node->RegisterWithGameInstance(World);
			
			return Node;
		}
	}

	return nullptr;
}

void UAsyncAction_PushSoftWidget::Activate()
{
	Super::Activate();

	UFrontendSubsystem* FrontendSubsystem = UFrontendSubsystem::Get(CachedOwningWorld.Get());

	FrontendSubsystem->PushSoftWidgetToStackAsync(CachedWidgetStackTag, CachedSoftWidgetClass,
		[this](EAsyncPushWidgetState InPushState, ULeActivatableWidget* PushedWidget)
		{
			switch (InPushState)
			{
			case EAsyncPushWidgetState::OnCreatedBeforePush:
				PushedWidget->SetOwningPlayer(CachedOwningPC.Get());
				OnWidgetCreatedBeforePush.Broadcast(PushedWidget);
				break;
			case EAsyncPushWidgetState::AfterPush:
				AfterPush.Broadcast(PushedWidget);
				if (bCachedFocusOnNewlyPushedWidget)
				{
					if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
					{
						WidgetToFocus->SetFocus();
					}
				}

				SetReadyToDestroy();
				
				break;
				
			default:
				break;
			}
		}
	);
}
