// bong9 All Rights Reserved

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
