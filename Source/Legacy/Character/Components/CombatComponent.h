// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Legacy/Types/LeTypes.h"
#include "Legacy/Character/Components/Data/CombatComponentData.h"
#include "CombatComponent.generated.h"

class ULeAbilitySystemComponent;
class ALeWeapon;
class ULeWeaponItem;
class ALeCollision;
class ALeCharacter;

struct FCollisionActivationData;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent), ShowCategories=("Combat|Weapons"))
class LEGACY_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	bool SpawnAndInitWeapons();

	UFUNCTION(BlueprintCallable)
	ALeCollision* SpawnAndActivateCollision(const FCollisionActivationData& CollisionData);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|Weapons", meta=(DisplayPriority="1", ShowOnlyInnerProperties))
	TArray<TSubclassOf<ULeWeaponItem>> WeaponItemClasses;

	void BindTagChangedCallbacks();

	UPROPERTY(EditAnywhere, meta=(DisplayPriority="1", ShowOnlyInnerProperties))
	TArray<FEffectHandlingInfo> TagBindings;

private:
	void ActivateAbilities(FEffectHandlingInfo& Info, FGameplayTag Tag, ULeAbilitySystemComponent* ASC);
	void RemoveAbilities  (FEffectHandlingInfo& Info, FGameplayTag Tag, ULeAbilitySystemComponent* ASC);
	void ApplyEffects     (FEffectHandlingInfo& Info, FGameplayTag Tag, ULeAbilitySystemComponent* ASC);

	TMap<FGameplayTag, FTimerHandle> TimerHandlesAbilities;
	TMap<FGameplayTag, FTimerHandle> TimerHandlesRemove;
	TMap<FGameplayTag, FTimerHandle> TimerHandlesApply;

	template<typename Func>
	void ExecuteWithDelay(Func InFunction, float Delay, FTimerHandle& TimerHandle);
	
	void HandleEquipWeapon(ALeWeapon* Weapon);

	void HandleTagsChanged(FGameplayTag Tag, int32 NewCount);

	UPROPERTY(Transient, ReplicatedUsing=OnRep_MainHandWeapon, VisibleAnywhere)
	TObjectPtr<ALeWeapon> MainHandWeapon;

	UPROPERTY(Transient, ReplicatedUsing=OnRep_OffHandWeapon, VisibleAnywhere)
	TObjectPtr<ALeWeapon> OffHandWeapon;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Collision")
	TSubclassOf<ALeCollision> CollisionActorClass;

	UFUNCTION()
	void OnRep_MainHandWeapon();

	UFUNCTION()
	void OnRep_OffHandWeapon();
	
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
