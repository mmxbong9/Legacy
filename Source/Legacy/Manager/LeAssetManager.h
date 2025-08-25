// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "LeAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class LEGACY_API ULeAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static ULeAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
