// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "MotionWarpingComponent.h"
#include "Legacy/Interfaces/CharacterInterface.h"
#include "Legacy/Types/LeTypes.h"

#include "LeCharacter.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UCombatComponent;
class UHealthComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UMotionWarpingComponent;
class UNiagaraSystem;
class UParkourComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UStaticMeshComponent;
class UWidgetComponent;

struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChanged, EEquipWeapon, NewWeapon);

UCLASS(Blueprintable)
class LEGACY_API ALeCharacter : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	ALeCharacter();

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnWeaponChanged OnWeaponChanged;

	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetHealthBar() const { return HealthBar; }
	
	FORCEINLINE TObjectPtr<UWidgetComponent> GetShieldWidget() const { return ShieldWidget; }
	FORCEINLINE TObjectPtr<UWidgetComponent> GetPistolWidget() const { return PistolWidget; }
	FORCEINLINE TObjectPtr<UWidgetComponent> GetRifleWidget()  const { return RifleWidget; }

	FORCEINLINE EEquipWeapon GetCurrentEquipWeapon() const { return EquipWeapon; }
	FORCEINLINE EMoveType    GetCurrentMoveType()    const { return CurrentMoveType; }
	FORCEINLINE bool         GetIsAiming()           const { return bIsAiming; }

	void SetIsAiming(const bool bNewIsAiming);
	UFUNCTION(Server, Reliable)
	void Server_SetIsAiming(const bool bNewAiming);

	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetPistolSkeletalMesh() const { return Pistol; }
	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetRifleSkeletalMesh()  const { return Rifle; }

	FORCEINLINE TArray<TObjectPtr<USoundBase>> GetConcreteImpactSounds() const { return ConcreteImpactSounds; }
	FORCEINLINE TArray<TObjectPtr<USoundBase>> GetGlassImpactSounds()    const { return GlassImpactSounds; }
	
	FORCEINLINE TObjectPtr<UNiagaraSystem> GetFireTracerEffect()     const { return FireTracerEffect; }
	
	FORCEINLINE TObjectPtr<UNiagaraSystem> GetConcreteImpactEffect() const { return ConcreteImpactEffect; }
	FORCEINLINE TObjectPtr<UNiagaraSystem> GetGlassImpactEffect()    const { return GlassImpactEffect; }
	
	FORCEINLINE TObjectPtr<UAnimMontage> GetPistolFireMontage()   const { return PistolFireMontage; }
	FORCEINLINE TObjectPtr<UAnimMontage> GetPistolReloadMontage() const { return PistolReloadMontage; }

	FORCEINLINE TObjectPtr<UAnimSequence> GetPistolFireAnimSequence()   const { return PistolFireAnimSequence; }
	FORCEINLINE TObjectPtr<UAnimSequence> GetPistolReloadAnimSequence() const { return PistolReloadAnimSequence; }
	FORCEINLINE TObjectPtr<USoundBase>    GetPistolFireSound()          const { return PistolFireSound; }
	
	FORCEINLINE TObjectPtr<UAnimMontage> GetRifleFireMontage()   const { return RifleFireMontage; }
	FORCEINLINE TObjectPtr<UAnimMontage> GetRifleReloadMontage() const { return RifleReloadMontage; }

	FORCEINLINE TObjectPtr<UAnimSequence> GetRifleFireAnimSequence()   const { return RifleFireAnimSequence; }
	FORCEINLINE TObjectPtr<UAnimSequence> GetRifleReloadAnimSequence() const { return RifleReloadAnimSequence; }
	FORCEINLINE TObjectPtr<USoundBase>    GetRifleFireSound()          const { return RifleFireSound; }
	
	static UMotionWarpingComponent* FindMotionWarpingComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UMotionWarpingComponent>() : nullptr); }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void NotifyControllerChanged() override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:
	virtual void Tick(float InDeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InPlayerInputComponent) override;

private:
	//- Character Interface
	virtual bool DoDamage_Implementation(FName InHitBoneName, AActor* InDamageInstigator) override;
	virtual void IncreaseHealth_Implementation    (float InAmount) override;
	virtual void DecreaseHealth_Implementation    (float InAmount) override;
	virtual void IncreaseShield_Implementation    (float InAmount) override;
	virtual void IncreasePistolClip_Implementation(int   InAmount) override;
	virtual void IncreaseRifleClip_Implementation (int   InAmount) override;
	//- Character Interface
	
private:
	float GetGroundDistance() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta=(AllowPrivateAccess=true)) TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere,  BlueprintReadOnly)  TObjectPtr<UCombatComponent> CombatComponent;
	UPROPERTY(VisibleAnywhere,  BlueprintReadOnly)  TObjectPtr<UHealthComponent> HealthComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) TSubclassOf<UParkourComponent>      ParkourClass;
	UPROPERTY(VisibleAnywhere,  BlueprintReadOnly)  TObjectPtr<UParkourComponent>       Parkour;
	UPROPERTY(VisibleAnywhere,  BlueprintReadOnly)  TObjectPtr<UMotionWarpingComponent> MotionWarping;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TObjectPtr<UStaticMeshComponent> Torch;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TObjectPtr<UStaticMeshComponent> TorchHolder;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TObjectPtr<UStaticMeshComponent> PistolHolster;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TObjectPtr<UStaticMeshComponent> Helmet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TObjectPtr<UStaticMeshComponent> HealthBar;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="01.Settings|Weapon") TObjectPtr<USkeletalMeshComponent> Rifle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="01.Settings|Weapon") TObjectPtr<USkeletalMeshComponent> Pistol;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="01.Settings|Widget") TObjectPtr<UWidgetComponent>  PistolWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="01.Settings|Widget") TObjectPtr<UWidgetComponent>  RifleWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="01.Settings|Widget") TObjectPtr<UWidgetComponent>  ShieldWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="01.Settings|Widget") TObjectPtr<UMaterialInstance> ShieldWidgetMaterial;
	
	UPROPERTY(EditAnywhere,    Category="01.Settings|Hud") TSubclassOf<UUserWidget> ViewPortCrossHairWidgetClass;
	UPROPERTY(VisibleAnywhere, Category="01.Settings|Hud") TObjectPtr<UUserWidget>  ViewPortCrossHairWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="01.Settings|NiagaraEffect") TObjectPtr<UNiagaraSystem> FireTracerEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="01.Settings|NiagaraEffect") TObjectPtr<UNiagaraSystem> ConcreteImpactEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="01.Settings|NiagaraEffect") TObjectPtr<UNiagaraSystem> GlassImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="01.Settings|Sounds") TArray<TObjectPtr<USoundBase>> ConcreteImpactSounds;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="01.Settings|Sounds") TArray<TObjectPtr<USoundBase>> GlassImpactSounds;
	
private:
	bool bCanInteract;

protected:
	UPROPERTY(EditAnywhere, Category="01.Settings|DataTable")
	TObjectPtr<UDataTable> WeaponMovementSettingDataTable;
	
	UPROPERTY(EditAnywhere, Category="01.Settings|Aim")
	TObjectPtr<UCurveFloat> AimCurve;
	
	UPROPERTY(EditAnywhere, Category="01.Settings|Weapon")
	FWeaponSocketNames WeaponSocketNames;
	
	UPROPERTY(EditAnywhere, Category="01.Settings|WeaponClass") TSubclassOf<UAnimInstance> UnArmedAnimClass;
	UPROPERTY(EditAnywhere, Category="01.Settings|WeaponClass") TSubclassOf<UAnimInstance> PistolAnimClass;
	UPROPERTY(EditAnywhere, Category="01.Settings|WeaponClass") TSubclassOf<UAnimInstance> RifleAnimClass;
	
	UPROPERTY(EditAnywhere, Category="01.Settings|Weapon|Pistol") TObjectPtr<UAnimMontage>  PistolFireMontage;
	UPROPERTY(EditAnywhere, Category="01.Settings|Weapon|Pistol") TObjectPtr<UAnimMontage>  PistolReloadMontage;
	UPROPERTY(EditAnywhere, Category="01.Settings|Weapon|Pistol") TObjectPtr<UAnimSequence> PistolFireAnimSequence;
	UPROPERTY(EditAnywhere, Category="01.Settings|Weapon|Pistol") TObjectPtr<UAnimSequence> PistolReloadAnimSequence;
	UPROPERTY(EditAnywhere, Category="01.Settings|Weapon|Pistol") TObjectPtr<USoundBase>    PistolFireSound;
	
	UPROPERTY(EditAnywhere, Category="01.Settings|Weapon|Rifle")  TObjectPtr<UAnimMontage>  RifleFireMontage;
	UPROPERTY(EditAnywhere, Category="01.Settings|Weapon|Rifle")  TObjectPtr<UAnimMontage>  RifleReloadMontage;
	UPROPERTY(EditAnywhere, Category="01.Settings|Weapon|Rifle")  TObjectPtr<UAnimSequence> RifleFireAnimSequence;
	UPROPERTY(EditAnywhere, Category="01.Settings|Weapon|Rifle")  TObjectPtr<UAnimSequence> RifleReloadAnimSequence;
	UPROPERTY(EditAnywhere, Category="01.Settings|Weapon|Rifle")  TObjectPtr<USoundBase>    RifleFireSound;
	
private:
	UFUNCTION(Server, Reliable)
	void Server_DoParkour();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DoParkour();
	
	void ChangeWeapon(const EEquipWeapon InWeapon);
	UFUNCTION(Server, Reliable)
	void Server_ChangeWeapon(const EEquipWeapon InWeapon);
	
	UFUNCTION()
	void OnRep_EquipWeapon();

	UPROPERTY(ReplicatedUsing=OnRep_EquipWeapon)
	EEquipWeapon EquipWeapon;

	void SetLinkAnimClassLayer(EEquipWeapon InWeapon) const;

	void UpdateMoveType(const EMoveType InMoveType);
	UFUNCTION(Server, Reliable)
	void Server_UpdateMoveType(const EMoveType InMoveType);
	
	UFUNCTION()
	void OnRep_MoveType();

	UPROPERTY(ReplicatedUsing=OnRep_MoveType)
	EMoveType CurrentMoveType;

	UPROPERTY(Replicated)
	FVector ReplicatedAcceleration;
	
	UFUNCTION()
	void OnRep_Aiming();

	UPROPERTY(ReplicatedUsing=OnRep_Aiming)
	bool bIsAiming;

	void AimStart();
	void AimEnd();

	FOnTimelineFloat AimInterpFunction{};
	FOnTimelineEvent AimTimeLineFinished{};
	
	UFUNCTION()
	void OnAimTimelineFloatReturn(const float InValue);

	UFUNCTION()
	void OnAimTimelineFinished();

	FTimeline AimTimeline;

protected:
	//~ Input Events
	void Input_SwitchWeapon(const FInputActionValue& InValue);
	void Input_Look        (const FInputActionValue& InValue);
	void Input_Move        (const FInputActionValue& InValue);
	void Input_AimStart();
	void Input_AimEnd();
	void Input_Crouch();
	void Input_JumpStart();
	void Input_JumpEnd();
	void Input_Fire();
	void Input_Reload();
	void Input_Interaction();
	//~ Input Events
	
protected:
	//~ Input Assets
	UPROPERTY(EditDefaultsOnly, Category="01.Settings|Input|Character") TObjectPtr<UInputMappingContext> CharacterIMC;
	UPROPERTY(EditDefaultsOnly, Category="01.Settings|Input|Character") TObjectPtr<UInputAction> AimIA;
	UPROPERTY(EditDefaultsOnly, Category="01.Settings|Input|Character") TObjectPtr<UInputAction> CrouchIA;
	UPROPERTY(EditDefaultsOnly, Category="01.Settings|Input|Character") TObjectPtr<UInputAction> FireIA;
	UPROPERTY(EditDefaultsOnly, Category="01.Settings|Input|Character") TObjectPtr<UInputAction> InteractionIA;
	UPROPERTY(EditDefaultsOnly, Category="01.Settings|Input|Character") TObjectPtr<UInputAction> JumpIA;
	UPROPERTY(EditDefaultsOnly, Category="01.Settings|Input|Character") TObjectPtr<UInputAction> LookIA;
	UPROPERTY(EditDefaultsOnly, Category="01.Settings|Input|Character") TObjectPtr<UInputAction> MoveIA;
	UPROPERTY(EditDefaultsOnly, Category="01.Settings|Input|Character") TObjectPtr<UInputAction> ReloadIA;
	UPROPERTY(EditDefaultsOnly, Category="01.Settings|Input|Character") TObjectPtr<UInputAction> SwitchWeaponIA;
	//~ Input Assets
};
