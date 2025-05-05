// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Legacy/Types/LeTypes.h"
#include "CombatComponent.generated.h"

enum class EEquipWeapon : uint8;
class ALeCharacter;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class LEGACY_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void InitializeComponent() override;

	void InitializeWeaponData();

	FWeaponData* GetCurrentWeaponData();

	UFUNCTION()
	void HandleWeaponChanged(EEquipWeapon NewWeapon);

public:
	void WeaponFire(const FVector& InCameraLoc, const FVector& InCameraForward);
	void FireLineTrace(const FVector& InStartLocation, const FVector& InDirection);
	
	UFUNCTION(Server, Reliable)
	void Server_WeaponFire(const FVector& InCameraLoc, const FVector& InCameraForward);

	UFUNCTION(NetMulticast, UnReliable)
	void Multicast_WeaponFire(const FVector& InCameraLoc, const FVector& InCameraForward);

	void Reload();

	UFUNCTION(Server, Reliable)
	void Server_Reload();

	UFUNCTION(NetMulticast, UnReliable)
	void Multicast_Reload();

	void IncreaseClip(const EEquipWeapon InWeapon, const int InAmount);
	void SetWeaponWidgetVisibility(const EEquipWeapon InWeapon, const bool bInVisibility=false);
	void UpdateWeaponWidget(const EEquipWeapon InWeapon, const bool bUseOnlyLocalPlayer = true);
	void ProcessWidgetByAnim() const;

private:
	void PlayImpactSound  (const FVector& InImpactLocation, const UPhysicalMaterial* InPhysicalMaterial) const;
	void SpawnImpactEffect(const FVector& InImpactLocation, const FVector& InImpactNormal, const UPhysicalMaterial* InPhysicalMaterial) const;
	void SpawnFireTracer  (const FVector& InImpactLocation, USceneComponent* InWeaponParentComponent, const FName InSpawnSocketName = FName("") ) const;
	
	bool bCanFire;

	FTimerHandle FireTimerHandle;
	FTimerHandle ReloadTimerHandle;

	UPROPERTY(EditAnywhere, Category="01.Settings|Debug")
	bool bUseDebugFireTrace;

	UPROPERTY()
	TObjectPtr<ALeCharacter> LeCharacter;

	UPROPERTY()
	TMap<EEquipWeapon, FWeaponData> WeaponDataMap;
};
