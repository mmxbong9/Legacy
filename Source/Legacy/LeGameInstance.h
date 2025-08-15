// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEGACY_API ULeGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CancelPending();
};
