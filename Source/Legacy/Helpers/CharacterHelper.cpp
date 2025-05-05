// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterHelper.h"

UEnhancedInputLocalPlayerSubsystem* UCharacterHelper::GetInputLocalPlayerSubsystem(const TObjectPtr<AController>& InController)
{
	if (const APlayerController* PC = Cast<APlayerController>(InController))
	{
		if (const ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
				return Subsystem;
		}
	}

	return nullptr;
}
