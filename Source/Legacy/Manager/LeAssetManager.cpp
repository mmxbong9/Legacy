// bong9 All Rights Reserved

#include "LeAssetManager.h"

#include "Legacy/GameplayTags/LeGameplayTags.h"

ULeAssetManager& ULeAssetManager::Get()
{
	check(GEngine);
	ULeAssetManager* AssetManager = Cast<ULeAssetManager>(GEngine->AssetManager);
	return *AssetManager;
}

void ULeAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FLeGameplayTags::InitializeNativeGameplayTags();
}
