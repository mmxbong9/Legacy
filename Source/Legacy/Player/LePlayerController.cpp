// Fill out your copyright notice in the Description page of Project Settings.

#include "LePlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Legacy/UI/Widgets/LePrimaryLayoutWidget.h"
#include "Legacy/Settings/LegacyGameplayTags.h"
#include "Legacy/Settings/LegacyGameUserSettings.h"
#include "Legacy/Helpers/LegacyFunctionLibrary.h"
#include "Legacy/SubSystems/FrontendSubsystem.h"
#include "Legacy/UI/ASyncActions/AsyncAction_PushSoftWidget.h"

ALePlayerController::ALePlayerController()
{
}

void ALePlayerController::OnLoadingScreenActivated_Implementation()
{
	
}

void ALePlayerController::OnLoadingScreenDeactivated_Implementation()
{
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

void ALePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	
}
