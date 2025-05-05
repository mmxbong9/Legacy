// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WidgetInterface.generated.h"

UINTERFACE()
class UWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class LEGACY_API IWidgetInterface
{
	GENERATED_BODY()
	
public:
	virtual void UpdateBulletAmount(float InAmount) = 0;
	virtual void UpdateClipsAmount (float InAmount) = 0;
	virtual void UpdateShieldAmount(float InAmount) = 0;
};
