// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleInteractionComponent.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class LEGACY_API UVehicleInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UVehicleInteractionComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float InDeltaTime, ELevelTick InTickType, FActorComponentTickFunction* InThisTickFunction) override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void InteractionStarted(AActor* InActor);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta=(AllowPrivateAccess=true))
	bool bVehicleInCharacter;
};
