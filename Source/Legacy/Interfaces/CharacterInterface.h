// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

UINTERFACE(Blueprintable)
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class LEGACY_API ICharacterInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool DoDamage(FName InHitBoneName, AActor* InDamageInstigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void IncreaseHealth(float InAmount);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DecreaseHealth(float InAmount);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void IncreaseShield(float InAmount);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void IncreasePistolClip(int InAmount);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void IncreaseRifleClip(int InAmount);
};
