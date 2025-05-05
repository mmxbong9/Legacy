// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LePlayerController.generated.h"

class UEnhancedInputLocalPlayerSubsystem;

/**
 *
 */
UCLASS()
class LEGACY_API ALePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALePlayerController();
};
