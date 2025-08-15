// Fill out your copyright notice in the Description page of Project Settings.

#include "LePlayerController.h"

#include "CommonActivatableWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Legacy/Character/LeCharacter.h"
#include "Legacy/UI/Widgets/LePrimaryLayoutWidget.h"
#include "Legacy/Settings/LegacyGameplayTags.h"
#include "Legacy/Settings/LegacyGameUserSettings.h"
#include "Legacy/Helpers/LegacyFunctionLibrary.h"
#include "Legacy/SubSystems/FrontendSubsystem.h"
#include "Legacy/UI/ASyncActions/AsyncAction_PushSoftWidget.h"
#include "Legacy/UI/Widgets/LeActivatableWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

ALePlayerController::ALePlayerController()
{
}

void ALePlayerController::OnLoadingScreenActivated_Implementation()
{
	// TArray<UUserWidget*> FoundWidgets;
	// UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UUserWidget::StaticClass(), false);
	//
	// for (UUserWidget* Widget : FoundWidgets)
	// {
	// 	if (Widget && Widget->IsInViewport())
	// 	{
	// 		Widget->RemoveFromParent();
	// 	}
	// }
}

void ALePlayerController::OnLoadingScreenDeactivated_Implementation()
{
	// 타이틀 레벨 에서만 최초 시행하며, 인게임 중일때 메인 메뉴 호출은 ToggleMainMenu 함수 호출에 의해 실행 된다.
	if (!IsValid(GetPawn()) || GetPawn()->IsA<ALeCharacter>())
		return;
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName("Default"), Actors);
	
	if (!Actors.IsEmpty())
	{
		if (ACameraActor* CameraActor = Cast<ACameraActor>(Actors[0]))
		{
			if (UCameraComponent* CameraComponent = CameraActor->GetCameraComponent()) // set to full screen.
				CameraComponent->SetConstraintAspectRatio(false);
			
			SetViewTarget(CameraActor);
		}
	}
	
	if (ULePrimaryLayoutWidget* PrimaryWidgetInstance = CreateWidget<ULePrimaryLayoutWidget>(this,
		LoadObject<UClass>(nullptr, *PrimaryLayoutPath)))
	{
		PrimaryWidgetInstance->AddToViewport();
		UE_LOG(LogTemp, Log, TEXT("Widget instance created and added to viewport."));
		
		UFrontendSubsystem::Get(this)->RegisterCreatedPrimaryLayoutWidget(PrimaryWidgetInstance);
	}
	
	if (UAsyncAction_PushSoftWidget* Action = UAsyncAction_PushSoftWidget::PushSoftWidget(this, this,
		ULegacyFunctionLibrary::GetSoftWidgetClassByTag(LegacyGameplayTags::Widget_PressAnyKeyScreen),
		LegacyGameplayTags::WidgetStack_MainMenu, true))
	{
		Action->Activate();
	}

	if (ULegacyGameUserSettings* GameUserSettings = ULegacyGameUserSettings::Get())
	{
		if (GameUserSettings->GetLastCPUBenchmarkResult() == -1.f || GameUserSettings->GetLastGPUBenchmarkResult() == -1.f)
		{
			GameUserSettings->RunHardwareBenchmark();
			GameUserSettings->ApplyHardwareBenchmarkResults();
		}
	}
}

void ALePlayerController::ToggleMainMenu()
{
	// 인게임 플레이시 메인메뉴가 떠있다면 제거 하고 게임모드로 변경.
	if (ULePrimaryLayoutWidget* PrimaryLayout = UFrontendSubsystem::Get(this)->GetCreatedPrimaryLayout() )
	{
		if (UCommonActivatableWidgetContainerBase* WidgetStack = PrimaryLayout->FindWidgetStackByTag(LegacyGameplayTags::WidgetStack_MainMenu))
		{
			if (UCommonActivatableWidget* ActivatableWidget = WidgetStack->GetActiveWidget())
			{
				WidgetStack->ClearWidgets();
				WidgetStack->RemoveFromParent();

				UE_LOG(LogTemp, Log, TEXT("RemoveFromParent MainMenu Widget."));
				
				SetInputMode(FInputModeGameOnly());

				return;
			}
		}
	}

	// 인게임 플레이시 메인메뉴가 없다면 메인 메뉴를 띄우고 UI모드로 변경.
	if (UFrontendSubsystem* Frontend = UFrontendSubsystem::Get(this))
	{
		// 등록된 Primary Layout Widget 제거
		if (ULePrimaryLayoutWidget* PrimaryWidgetInstance = Frontend->GetCreatedPrimaryLayout())
		{
			PrimaryWidgetInstance->RemoveFromParent();
			Frontend->UnregisterPrimaryLayoutWidget();
			UE_LOG(LogTemp, Log, TEXT("Primary layout widget removed from viewport."));
		}
	}
	
	if (ULePrimaryLayoutWidget* PrimaryWidgetInstance = CreateWidget<ULePrimaryLayoutWidget>(this,
			LoadObject<UClass>(nullptr, *PrimaryLayoutPath)))
	{
		PrimaryWidgetInstance->AddToViewport();
		UE_LOG(LogTemp, Log, TEXT("Widget instance created and added to viewport."));
		
		UFrontendSubsystem::Get(this)->RegisterCreatedPrimaryLayoutWidget(PrimaryWidgetInstance);
	}
	
	if (UAsyncAction_PushSoftWidget* Action = UAsyncAction_PushSoftWidget::PushSoftWidget(this, this,
		ULegacyFunctionLibrary::GetSoftWidgetClassByTag(LegacyGameplayTags::Widget_MainMenuScreen),
		LegacyGameplayTags::WidgetStack_MainMenu, true, false))
	{
		Action->Activate();
		
		SetInputMode(FInputModeUIOnly());
	}
}

void ALePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
}
