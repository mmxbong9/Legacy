#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "CharacterHelper.generated.h"

UCLASS()
class LEGACY_API UCharacterHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UEnhancedInputLocalPlayerSubsystem* GetInputLocalPlayerSubsystem(const TObjectPtr<AController>& InController);
};
