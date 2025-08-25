// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "LeCharacter.h"
#include "LePlayerCharacter.generated.h"

UCLASS()
class LEGACY_API ALePlayerCharacter : public ALeCharacter
{
	GENERATED_BODY()

public:
	ALePlayerCharacter();

	//~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	//~ End APawn Interface

protected:
	virtual void BeginPlay() override;

	//~ Begin ALeCharacter Interface
	virtual void InitializeAbilitySystem() override;
	//~ End ALeCharacter Interface
	
};
