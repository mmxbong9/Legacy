// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "LeCharacter.h"
#include "LeEnemyCharacter.generated.h"

UCLASS()
class LEGACY_API ALeEnemyCharacter : public ALeCharacter
{
	GENERATED_BODY()

public:
	ALeEnemyCharacter();

protected:
	virtual void BeginPlay() override;

	//~ Begin ALeCharacter Interface
	virtual void InitializeAbilitySystem() override;
	//~ End ALeCharacter Interface
};
