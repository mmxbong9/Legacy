// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Components/VehicleInteractionComponent.h"
#include "LeVehicle.generated.h"

class ALeCharacter;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class LEGACY_API ALeVehicle : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	ALeVehicle();
	
	UFUNCTION(BlueprintPure, Category = "Interaction")
	ALeCharacter* GetInteractionCharacter() const;

	virtual void PossessedBy(AController* InNewController) override;
	virtual void UnPossessed() override;

	virtual void NotifyControllerChanged() override;

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyControllerChangedForBP();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:
	virtual void Tick(float InDeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InPlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) TSubclassOf<UVehicleInteractionComponent> VehicleInteractionComponentClass;
	UPROPERTY(VisibleAnywhere,  BlueprintReadOnly)  TObjectPtr<UVehicleInteractionComponent>  VehicleInteractionComponent;

protected:
	//~ Vehicle Input Events
	void Input_Brake     (const FInputActionValue& InValue);
	void Input_HandBrake (const FInputActionValue& InValue);
	void Input_LookAround(const FInputActionValue& InValue);
	void Input_Steering  (const FInputActionValue& InValue);
	void Input_Throttle  (const FInputActionValue& InValue);
	void Input_VehicleExit();
	//~ Vehicle Input Events
	
private:
	//~ Vehicle Input Assets
	UPROPERTY(EditAnywhere, Category="01.Settings|Input|Vehicle", Meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputMappingContext> VehicleIMC;
	UPROPERTY(EditAnywhere, Category="01.Settings|Input|Vehicle", Meta=(AllowPrivateAccess=true)) TObjectPtr<UInputAction> BrakeIA;
	UPROPERTY(EditAnywhere, Category="01.Settings|Input|Vehicle", Meta=(AllowPrivateAccess=true)) TObjectPtr<UInputAction> HandBrakeIA;
	UPROPERTY(EditAnywhere, Category="01.Settings|Input|Vehicle", Meta=(AllowPrivateAccess=true)) TObjectPtr<UInputAction> LookAroundIA;
	UPROPERTY(EditAnywhere, Category="01.Settings|Input|Vehicle", Meta=(AllowPrivateAccess=true)) TObjectPtr<UInputAction> SteeringIA;
	UPROPERTY(EditAnywhere, Category="01.Settings|Input|Vehicle", Meta=(AllowPrivateAccess=true)) TObjectPtr<UInputAction> ThrottleIA;
	UPROPERTY(EditAnywhere, Category="01.Settings|Input|Vehicle", Meta=(AllowPrivateAccess=true)) TObjectPtr<UInputAction> VehicleExitIA;
	//~ Vehicle Input Assets

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta=(AllowPrivateAccess=true))
	TObjectPtr<ALeCharacter> InteractionCharacter;
};
