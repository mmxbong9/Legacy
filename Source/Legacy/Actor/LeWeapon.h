// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeWeapon.generated.h"

UCLASS()
class LEGACY_API ALeWeapon : public AActor
{
	GENERATED_BODY()

public:
	ALeWeapon();

	UFUNCTION(BlueprintCallable)
	void AttachToCharacter(const FName& AttachmentPoint);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Weapon)
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMesh;
};
