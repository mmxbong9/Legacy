// bong9 All Rights Reserved

#include "FrontendSubsystem.h"

#include "Engine/AssetManager.h"
#include "Legacy/Settings/LegacyGameplayTags.h"
#include "Legacy/UI/Widgets/LeActivatableWidget.h"
#include "Legacy/Helpers/DebugHelper.h"
#include "Legacy/Helpers/LegacyFunctionLibrary.h"
#include "Legacy/UI/Widgets/LeConfirmScreenWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

UFrontendSubsystem* UFrontendSubsystem::Get(const UObject* InWorldContextObject)
{
	if (GEngine)
	{
		const UWorld* World = GEngine->GetWorldFromContextObject(InWorldContextObject, EGetWorldErrorMode::Assert);
		
		return UGameInstance::GetSubsystem<UFrontendSubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

bool UFrontendSubsystem::ShouldCreateSubsystem(UObject* InOuter) const
{
	if (!CastChecked<UGameInstance>(InOuter)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}
	
	return false;
}

void UFrontendSubsystem::UnregisterPrimaryLayoutWidget()
{
	if (!IsValid(CreatedPrimaryLayout))
	{
		return;
	}

	// 1) 지역 변수로 보관하고, 즉시 널 클리어(재진입/레이스 방지)
	ULePrimaryLayoutWidget* Layout = CreatedPrimaryLayout;
	CreatedPrimaryLayout = nullptr;

	// 2) 메인 메뉴 스택 비우기 (뷰포트 부착 여부와 무관)
	if (IsValid(Layout))
	{
		if (UCommonActivatableWidgetContainerBase* Stack = Layout->FindWidgetStackByTag(LegacyGameplayTags::WidgetStack_MainMenu))
		{
			Stack->ClearWidgets(); // 스택 Pop 및 Activatable 종료
		}

		// 3) 뷰포트에서 제거 (없으면 noop)
		Layout->RemoveFromParent();

		UE_LOG(LogTemp, Log, TEXT("Primary layout widget removed and stack cleared."));
	}
}

void UFrontendSubsystem::RegisterCreatedPrimaryLayoutWidget(ULePrimaryLayoutWidget* InCreatedWidget)
{
	check(InCreatedWidget);
	
	CreatedPrimaryLayout = InCreatedWidget;
}

void UFrontendSubsystem::PushSoftWidgetToStackAsync(
	const FGameplayTag& InWidgetStackTag,
	TSoftClassPtr<ULeActivatableWidget> InSoftWidgetClass,
	TFunction<void(EAsyncPushWidgetState, ULeActivatableWidget*)> ASyncPushStateCallback
	)
{
	check(!InSoftWidgetClass.IsNull())

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		InSoftWidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[InSoftWidgetClass, this, InWidgetStackTag, ASyncPushStateCallback]()
			{
				UClass* LoadedWidgetClass = InSoftWidgetClass.Get();
				
				check(LoadedWidgetClass);
				
				UCommonActivatableWidgetContainerBase* WidgetStack = CreatedPrimaryLayout->FindWidgetStackByTag(InWidgetStackTag);

				ULeActivatableWidget* CreatedWidget = WidgetStack->AddWidget<ULeActivatableWidget>(
					LoadedWidgetClass,
					[ASyncPushStateCallback](ULeActivatableWidget& CreatedWidgetInstance)
					{
						ASyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
					}
				);

				ASyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
			}
		)
	);
}

void UFrontendSubsystem::PushConfirmScreenToPopupStackAsync(EConfirmScreenType InScreenType, const FText& InScreenTitle,
	const FText& InScreenMessage, TFunction<void(EConfirmScreenButtonType)> ButtonClickedCallback)
{
	ULeConfirmInfoObject* CreatedInfoObject = nullptr;
	
	switch (InScreenType)
	{
	case EConfirmScreenType::Ok:
		CreatedInfoObject = ULeConfirmInfoObject::CreateOkScreen(InScreenTitle, InScreenMessage);
		break;
	case EConfirmScreenType::YesNo:
		CreatedInfoObject = ULeConfirmInfoObject::CreateYesNoScreen(InScreenTitle, InScreenMessage);
		break;
	case EConfirmScreenType::OkCancel:
		CreatedInfoObject = ULeConfirmInfoObject::CreateOkCancelScreen(InScreenTitle, InScreenMessage);
		break;
	default: break;		
	}

	check(CreatedInfoObject);

	PushSoftWidgetToStackAsync(
		LegacyGameplayTags::WidgetStack_Popup,
		ULegacyFunctionLibrary::GetSoftWidgetClassByTag(LegacyGameplayTags::Widget_ConfirmScreen),
		[CreatedInfoObject, ButtonClickedCallback](EAsyncPushWidgetState InPushState, ULeActivatableWidget* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				ULeConfirmScreenWidget* CreatedConfirmScreen = CastChecked<ULeConfirmScreenWidget>(PushedWidget);
				CreatedConfirmScreen->InitConfirmScreen(CreatedInfoObject, ButtonClickedCallback);
			}
		}
	);
}

void UFrontendSubsystem::RemoveActivatedPopup()
{
	UCommonActivatableWidgetContainerBase* WidgetStack = CreatedPrimaryLayout->FindWidgetStackByTag(LegacyGameplayTags::WidgetStack_Popup);
	
	check(WidgetStack);

	if (UCommonActivatableWidget* ActivePopupWidget = WidgetStack->GetActiveWidget())
	{
		ActivePopupWidget->DeactivateWidget();
	}
}
