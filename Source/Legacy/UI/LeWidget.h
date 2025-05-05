// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Legacy/Interfaces/WidgetInterface.h"
#include "LeWidget.generated.h"

/**
 * 
 */
UCLASS()
class LEGACY_API ULeWidget : public UUserWidget, public IWidgetInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateBullet(float Amount);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateClips(float Amount);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateShield(float Amount);

	//- Widget Interface
	virtual void UpdateBulletAmount(float InAmount) override;
	virtual void UpdateClipsAmount (float InAmount) override;
	virtual void UpdateShieldAmount(float InAmount) override;
	//- Widget Interface
};
