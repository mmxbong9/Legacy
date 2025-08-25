// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "MotionWarpingComponent.h"
#include "Components/LeAbilitySystemComponent.h"
#include "Components/WidgetComponent.h"
#include "Legacy/Interfaces/CharacterInterface.h"
#include "Legacy/Interfaces/HitInterface.h"
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

UCLASS(Blueprintable, ShowCategories=("Combat|Abilities"))
class LEGACY_API ALeCharacter : public ACharacter, public ICharacterInterface, public IAbilitySystemInterface, public IHitInterface
{
	GENERATED_BODY()

public:
	ALeCharacter();

	// todo: 레벨은 PlayerState 에서 관리 되어야함. 레벨별 Attribute stat 값 계산을 위해 임시로 지정. 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CharacterLevel = 1.f;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnWeaponChanged OnWeaponChanged;

	FORCEINLINE UStaticMeshComponent* GetHealthBar() const { return HealthBar; }
	
	FORCEINLINE UWidgetComponent* GetShieldWidget() const { return ShieldWidget.Get(); }
	FORCEINLINE UWidgetComponent* GetPistolWidget() const { return PistolWidget.Get(); }
	FORCEINLINE UWidgetComponent* GetRifleWidget()  const { return RifleWidget.Get(); }

	FORCEINLINE EEquipWeapon GetCurrentEquipWeapon() const { return EquipWeapon; }
	FORCEINLINE EMoveType    GetCurrentMoveType()    const { return CurrentMoveType; }
	FORCEINLINE bool         GetIsAiming()           const { return bIsAiming; }

	void SetIsAiming(const bool bNewIsAiming);
	UFUNCTION(Server, Reliable)
	void Server_SetIsAiming(const bool bNewAiming);

	FORCEINLINE const TArray<TObjectPtr<USoundBase>>& GetConcreteImpactSounds() const { return ConcreteImpactSounds; }
	FORCEINLINE const TArray<TObjectPtr<USoundBase>>& GetGlassImpactSounds()    const { return GlassImpactSounds; }
	
	FORCEINLINE USkeletalMeshComponent* GetPistolSkeletalMesh() const { return Pistol.Get(); }
	FORCEINLINE USkeletalMeshComponent* GetRifleSkeletalMesh()  const { return Rifle.Get(); }

	FORCEINLINE UCombatComponent* GetCombatComponent() const { return CombatComponent.Get(); }
	
	FORCEINLINE UNiagaraSystem* GetFireTracerEffect()         const { return FireTracerEffect.Get(); }
	FORCEINLINE UNiagaraSystem* GetConcreteImpactEffect()     const { return ConcreteImpactEffect.Get(); }
	FORCEINLINE UNiagaraSystem* GetGlassImpactEffect()        const { return GlassImpactEffect.Get(); }
	
	FORCEINLINE UAnimMontage*   GetPistolFireMontage()        const { return PistolFireMontage.Get(); }
	FORCEINLINE UAnimMontage*   GetPistolReloadMontage()      const { return PistolReloadMontage.Get(); }
	FORCEINLINE UAnimSequence*  GetPistolFireAnimSequence()   const { return PistolFireAnimSequence.Get(); }
	FORCEINLINE UAnimSequence*  GetPistolReloadAnimSequence() const { return PistolReloadAnimSequence.Get(); }
	FORCEINLINE USoundBase*     GetPistolFireSound()          const { return PistolFireSound.Get(); }
	FORCEINLINE UAnimMontage*   GetRifleFireMontage()         const { return RifleFireMontage.Get(); }
	FORCEINLINE UAnimMontage*   GetRifleReloadMontage()       const { return RifleReloadMontage.Get(); }
	FORCEINLINE UAnimSequence*  GetRifleFireAnimSequence()    const { return RifleFireAnimSequence.Get(); }
	FORCEINLINE UAnimSequence*  GetRifleReloadAnimSequence()  const { return RifleReloadAnimSequence.Get(); }
	FORCEINLINE USoundBase*     GetRifleFireSound()           const { return RifleFireSound.Get(); }
	
	static UMotionWarpingComponent* FindMotionWarpingComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UMotionWarpingComponent>() : nullptr); }

	//~ Begin AActor Interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void NotifyControllerChanged() override;
	//~ End AActor Interface

	//~ Begin IHitInterface
	virtual void PushActor_Implementation(const FVector& Direction, const float Strength) override;
	//~ End IHitInterface

	//~ Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent.Get(); }
	//~ End IAbilitySystemInterface

protected:
	//~ Begin AActor Interface
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	//~ End AActor Interface

	virtual void InitializeAbilitySystem() PURE_VIRTUAL(ThisClass::InitializeAbilitySystem, );
	virtual void InitializeDefaultAttributes();

	void ApplyEffectToTarget(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level);

	void AddAbilities() const;

public:
	//~ Begin APawn Interface
	virtual void Tick(float InDeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InPlayerInputComponent) override;
	//~ Begin APawn Interface

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="01.Settings|Weapon") TObjectPtr<USkeletalMeshComponent> Hammer;
	
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
	UPROPERTY(EditAnywhere, Category="01.Settings|WeaponClass") TSubclassOf<UAnimInstance> HammerAnimClass;
	
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

	UPROPERTY(EditAnywhere, Category="Combat|Abilities", meta=(DisplayPriority="1", ShowOnlyInnerProperties))
	TArray<TSubclassOf<ULeGameplayAbility>> CommonAbilities;

	UPROPERTY(EditAnywhere, Category="Combat|Abilities", meta=(DisplayPriority="1", ShowOnlyInnerProperties))
	TArray<TSubclassOf<ULeGameplayAbility>> DestroyerAbilities;

	UPROPERTY(EditAnywhere, Category="Combat|Attributes", meta=(DisplayPriority="1", ShowOnlyInnerProperties))
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	UPROPERTY(EditAnywhere, Category="Combat|Attributes", meta=(DisplayPriority="1", ShowOnlyInnerProperties))
	TSubclassOf<UGameplayEffect> MainAttributes;

	UPROPERTY(EditAnywhere, Category="Combat|Attributes", meta=(DisplayPriority="1", ShowOnlyInnerProperties))
	TSubclassOf<UGameplayEffect> SecondaryAttributes;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
private:
	UFUNCTION(Server, Reliable)
	void Server_DoParkour();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DoParkour();

	void Process_SelectWeapon(const EEquipWeapon InWeapon);
	
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
	void Input_SwitchWeapon();
	void Input_SelectWeapon1();
	void Input_SelectWeapon2();
	void Input_SelectWeapon3();
	void Input_SelectWeapon4();
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
	void Input_MainMenu();
	void AbilityAction(FGameplayTag ActionTag, bool bPressed);
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
	UPROPERTY(EditDefaultsOnly, Category="01.Settings|Input|Character") TObjectPtr<UInputAction> SelectWeapon1IA;
	UPROPERTY(EditDefaultsOnly, Category="01.Settings|Input|Character") TObjectPtr<UInputAction> SelectWeapon2IA;
	UPROPERTY(EditDefaultsOnly, Category="01.Settings|Input|Character") TObjectPtr<UInputAction> SelectWeapon3IA;
	UPROPERTY(EditDefaultsOnly, Category="01.Settings|Input|Character") TObjectPtr<UInputAction> SelectWeapon4IA;
	UPROPERTY(EditDefaultsOnly, Category="01.Settings|Input|Character") TObjectPtr<UInputAction> MainMenuIA;
	//~ Input Assets

	template<class TargetClass, typename TargetFunction>
	void BindTaggedInputActions(TMap<TObjectPtr<UInputAction>, FGameplayTag>& InputTagsMap, TargetClass* TargetObject, TargetFunction CallBack);

private:
	UPROPERTY(EditAnywhere, Category=Inpuit)
	TMap<TObjectPtr<UInputAction>, FGameplayTag> InputTagsMap;
};
