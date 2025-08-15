// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Legacy/Interfaces/LegacyLoadingScreenInterface.h"
#include "LePlayerController.generated.h"

class UAsyncAction_PushSoftWidget;
class ULeActivatableWidget;
class UEnhancedInputLocalPlayerSubsystem;
class ULePrimaryLayoutWidget;

/**
 *
 */
UCLASS()
class LEGACY_API ALePlayerController : public APlayerController, public ILegacyLoadingScreenInterface
{
	GENERATED_BODY()

public:
	ALePlayerController();

	//~ Begin ILegacyLoadingScreenInterface Interface
	virtual void OnLoadingScreenActivated_Implementation() override;
	virtual void OnLoadingScreenDeactivated_Implementation() override;
	//~ End ILegacyLoadingScreenInterface Interface

	void ToggleMainMenu();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	const FString PrimaryLayoutPath = TEXT("/Game/UI/Widgets/WBP_PrimaryLayout.WBP_PrimaryLayout_C");
};
