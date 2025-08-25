// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "LeItem.h"
#include "LeWeaponItem.generated.h"

class ALeWeapon;

/**
 * 
 */
UCLASS()
class LEGACY_API ULeWeaponItem : public ULeItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ALeWeapon> WeaponClass;
};
