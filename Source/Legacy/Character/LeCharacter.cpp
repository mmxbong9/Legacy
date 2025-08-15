// Fill out your copyright notice in the Description page of Project Settings.

#include "LeCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ParkourComponent.h"
#include "Components/CombatComponent.h"
#include "Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Legacy/Legacy.h"
#include "Legacy/Helpers/CharacterHelper.h"
#include "Legacy/Interfaces/AnimInterface.h"
#include "Legacy/Player/LePlayerController.h"
#include "Legacy/Types/LeTypes.h"
#include "Legacy/UI/LeWidget.h"

#pragma region Constructors
ALeCharacter::ALeCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bCanInteract = true;
	
	bIsAiming       = false;
	CurrentMoveType = EMoveType::Jogging;
	EquipWeapon     = EEquipWeapon::UnArmed;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	HealthComponent->SetIsReplicated(true);
	
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat"));
	CombatComponent->SetIsReplicated(true);
	
	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->TargetArmLength = 350.f;
	SpringArm->SocketOffset = FVector(50, 80, 40);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 10.f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 10.f;

	GetCharacterMovement()->SetCrouchedHalfHeight(60.0f);
	GetCharacterMovement()->MaxWalkSpeedCrouched = 250.f;
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	if (ConcreteImpactSounds.IsEmpty())
	{
		LOAD_ASSET_ADD_TO_ARRAY(ConcreteImpactSounds, USoundBase, "/Game/ALS/MetaSound/MSP_ImpactPlaster");
		LOAD_ASSET_ADD_TO_ARRAY(ConcreteImpactSounds, USoundBase, "/Game/ALS/MetaSound/MSP_ImpactPlasterDebries");
	}

	if (GlassImpactSounds.IsEmpty())
	{
		LOAD_ASSET_ADD_TO_ARRAY(GlassImpactSounds, USoundBase, "/Game/ALS/MetaSound/MSP_ImpactGlass");
		LOAD_ASSET_ADD_TO_ARRAY(GlassImpactSounds, USoundBase, "/Game/ALS/MetaSound/MSP_ImpactGlassDebries");
	}

	LOAD_OBJECT_ASSIGN(FireTracerEffect,        UNiagaraSystem, "/Game/ALS/Effects/Particles/Weapons/NS_WeaponFire_Tracer");
	LOAD_OBJECT_ASSIGN(ConcreteImpactEffect,    UNiagaraSystem, "/Game/ALS/Effects/Particles/Impacts/NS_ImpactConcrete");
	LOAD_OBJECT_ASSIGN(GlassImpactEffect,       UNiagaraSystem, "/Game/ALS/Effects/Particles/Impacts/NS_ImpactGlass");

	LOAD_OBJECT_ASSIGN(AimCurve,                UCurveFloat,    "/Game/Le/Curves/CF_Aim");

	LOAD_OBJECT_ASSIGN(PistolFireMontage,       UAnimMontage,   "/Game/ALS/Animations/Pistol/Gun/AM_MM_Pistol_Fire");
	LOAD_OBJECT_ASSIGN(PistolReloadMontage,     UAnimMontage,   "/Game/ALS/Animations/Pistol/Gun/AM_MM_Pistol_Reload");
	LOAD_OBJECT_ASSIGN(PistolFireAnimSequence,  UAnimSequence,  "/Game/ALS/Animations/Pistol/Gun/Weap_Pistol_Fire");
	LOAD_OBJECT_ASSIGN(PistolReloadAnimSequence,UAnimSequence,  "/Game/ALS/Animations/Pistol/Gun/Weap_Pistol_Reload");
	LOAD_OBJECT_ASSIGN(PistolFireSound,         USoundBase,     "/Game/ALS/MetaSound/MSP_PistolFire");

	LOAD_OBJECT_ASSIGN(RifleFireMontage,        UAnimMontage,   "/Game/ALS/Animations/Rifle/Gun/AM_MM_Rifle_Fire");
	LOAD_OBJECT_ASSIGN(RifleReloadMontage,      UAnimMontage,   "/Game/ALS/Animations/Rifle/Gun/AM_MM_Rifle_Reload");
	LOAD_OBJECT_ASSIGN(RifleFireAnimSequence,   UAnimSequence,  "/Game/ALS/Animations/Rifle/Gun/Weap_Rifle_Fire");
	LOAD_OBJECT_ASSIGN(RifleReloadAnimSequence, UAnimSequence,  "/Game/ALS/Animations/Rifle/Gun/Weap_Rifle_Reload");
	LOAD_OBJECT_ASSIGN(RifleFireSound,          USoundBase,     "/Game/ALS/MetaSound/MSP_RifleFire");

	LOAD_ASSET_SET_SKELETAL(GetMesh(), "/Game/ALS/Characters/Heroes/Mannequin/Meshes/SKM_Manny");
		
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
		
	Torch         = UCharacterHelper::CreateStaticMeshWithAsset(this, TEXT("Torch"),         GetMesh(), TEXT("Torch"),        TEXT("/Game/ALS/UI/StaticMeshes/Torch/SM_Torch"));
	TorchHolder   = UCharacterHelper::CreateStaticMeshWithAsset(this, TEXT("TorchHolder"),   GetMesh(), TEXT("TorchHolder")  ,TEXT("/Game/ALS/UI/StaticMeshes/Torch/SM_TorchHolder"));
	PistolHolster = UCharacterHelper::CreateStaticMeshWithAsset(this, TEXT("PistolHolster"), GetMesh(), TEXT("PistolHolster"),TEXT("/Game/ALS/UI/StaticMeshes/Holster/SM_Holster_R"));
	Helmet        = UCharacterHelper::CreateStaticMeshWithAsset(this, TEXT("Helmet"),        GetMesh(), TEXT("Helmet"),       TEXT("/Game/ALS/UI/StaticMeshes/Helmet/SM_Manny_Helmet"));
	HealthBar     = UCharacterHelper::CreateStaticMeshWithAsset(this, TEXT("HealthBar"),     GetMesh(), TEXT("HealthBar"),    TEXT("/Game/ALS/UI/StaticMeshes/HealthBar/SM_BodyHealthBar"));
		
	Pistol = UCharacterHelper::CreateSkeletalMeshWithAsset(this, TEXT("Pistol"), GetMesh(), TEXT("PistolUnEquipe"),TEXT("/Game/ALS/SkeletalMeshs/Pistol/SK_Pistol"));
	Rifle  = UCharacterHelper::CreateSkeletalMeshWithAsset(this, TEXT("Rifle"),  GetMesh(), TEXT("RifleUnEquipe"), TEXT("/Game/ALS/SkeletalMeshs/Rifle/SK_Rifle"));

	PistolWidget = UCharacterHelper::CreateWidgetCompWithClass(this, TEXT("PistolWidget"), Pistol, TEXT("Widget"), TEXT("/Game/ALS/UI/Widgets/WBP_LePistolUI"), EWidgetSpace::Screen, FVector2D(400, 80));
	RifleWidget  = UCharacterHelper::CreateWidgetCompWithClass(this, TEXT("RifleWidget"),   Rifle, TEXT("Widget"), TEXT("/Game/ALS/UI/Widgets/WBP_LeRifleUI"),  EWidgetSpace::Screen, FVector2D(400, 80));
	
	ShieldWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ShieldWidget"));
	ShieldWidget->SetupAttachment(HealthBar, NAME_None);
	ShieldWidget->SetDrawSize(FVector2D(50, 50));
	ShieldWidget->SetRelativeLocation(FVector(-2.6f, -1.5f, 19.1f));
	ShieldWidget->SetRelativeRotation(FRotator(0, -90, 0));
	ShieldWidget->SetRelativeScale3D(FVector(1.0f, 0.09f, 0.09f));
	
	LOAD_CLASS_ASSIGN(ViewPortCrossHairWidgetClass, UUserWidget, "/Game/ALS/UI/Widgets/WBP_CrossHair");
	
	LOAD_CLASS_ASSIGN(UnArmedAnimClass, UAnimInstance, "/Game/Le/ABP_LeUnArmed");
	LOAD_CLASS_ASSIGN(PistolAnimClass,  UAnimInstance, "/Game/Le/ABP_LePistol");
	LOAD_CLASS_ASSIGN(RifleAnimClass,   UAnimInstance, "/Game/Le/ABP_LeRifle");

	LOAD_OBJECT_ASSIGN(WeaponMovementSettingDataTable, UDataTable, "/Game/Le/DataTables/DT_WeaponMovement");

	LOAD_OBJECT_ASSIGN(CharacterIMC, UInputMappingContext, "/Game/ALS/Inputs/IMC_ALS");
	
	LOAD_OBJECT_ASSIGN(AimIA,           UInputAction, "/Game/ALS/Inputs/InputActions/IA_Aim");
	LOAD_OBJECT_ASSIGN(CrouchIA,        UInputAction, "/Game/ALS/Inputs/InputActions/IA_Crouch");
	LOAD_OBJECT_ASSIGN(FireIA,          UInputAction, "/Game/ALS/Inputs/InputActions/IA_Fire");
	LOAD_OBJECT_ASSIGN(InteractionIA,   UInputAction, "/Game/ALS/Inputs/InputActions/IA_Interaction");
	LOAD_OBJECT_ASSIGN(JumpIA,          UInputAction, "/Game/ALS/Inputs/InputActions/IA_Jump");
	LOAD_OBJECT_ASSIGN(LookIA,          UInputAction, "/Game/ALS/Inputs/InputActions/IA_Look");
	LOAD_OBJECT_ASSIGN(MoveIA,          UInputAction, "/Game/ALS/Inputs/InputActions/IA_Move");
	LOAD_OBJECT_ASSIGN(ReloadIA,        UInputAction, "/Game/ALS/Inputs/InputActions/IA_Reload");
	LOAD_OBJECT_ASSIGN(SelectWeapon1IA, UInputAction, "/Game/ALS/Inputs/InputActions/IA_SelectWeapon1");
	LOAD_OBJECT_ASSIGN(SelectWeapon2IA, UInputAction, "/Game/ALS/Inputs/InputActions/IA_SelectWeapon2");
	LOAD_OBJECT_ASSIGN(SelectWeapon3IA, UInputAction, "/Game/ALS/Inputs/InputActions/IA_SelectWeapon3");
	LOAD_OBJECT_ASSIGN(SwitchWeaponIA,  UInputAction, "/Game/ALS/Inputs/InputActions/IA_SwitchWeapon");
	LOAD_OBJECT_ASSIGN(MainMenuIA,      UInputAction, "/Game/ALS/Inputs/InputActions/IA_MainMenu");

	AimInterpFunction.BindDynamic  (this, &ALeCharacter::OnAimTimelineFloatReturn);
	AimTimeLineFinished.BindDynamic(this, &ALeCharacter::OnAimTimelineFinished);
}

void ALeCharacter::SetIsAiming(const bool bNewIsAiming)
{
	bIsAiming = bNewIsAiming;
}

void ALeCharacter::Server_SetIsAiming_Implementation(const bool bNewAiming)
{
	bIsAiming = bNewAiming;
	bIsAiming ? AimStart() : AimEnd();
}

#pragma endregion Constructors

void ALeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ALeCharacter, CurrentMoveType,        COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ALeCharacter, EquipWeapon,            COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ALeCharacter, bIsAiming,              COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ALeCharacter, ReplicatedAcceleration, COND_SkipOwner);
}

void ALeCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = UCharacterHelper::GetInputLocalPlayerSubsystem(Controller))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(CharacterIMC, 100);

		if (!IsValid(ViewPortCrossHairWidget.Get()))
		{
			ViewPortCrossHairWidget = CreateWidget(Cast<APlayerController>(Controller), ViewPortCrossHairWidgetClass);
			ViewPortCrossHairWidget->AddToViewport();
		}
	}
	else
	{
		if (IsValid(ViewPortCrossHairWidget.Get()))
		{
			ViewPortCrossHairWidget->RemoveFromParent();
			ViewPortCrossHairWidget = nullptr;
		}
	}
}

void ALeCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(ParkourClass))
	{
		Parkour = NewObject<UParkourComponent>(this, ParkourClass);
		Parkour->RegisterComponent();
		Parkour->SetIsReplicated(true);
	}
}

void ALeCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Default Class 및 Material 설정. (생성자에서 CDO 로 오브젝트 로딩시 랜더링 스레드에서 어설션 발생하므로 설정시 주의)  
	UClass* ShieldWidgetClass = LoadObject<UClass>(nullptr, TEXT("/Game/ALS/UI/Widgets/WBP_LeShield.WBP_LeShield_C"));
	if (IsValid(ShieldWidget) && ShieldWidgetClass && ShieldWidgetClass->IsChildOf(UUserWidget::StaticClass()))
	{
		ShieldWidget->SetWidgetClass(ShieldWidgetClass);
	}
	
	UMaterialInterface* ShieldWidgetClassMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/ALS/Material/Widget3DPassThrough_Translucent.Widget3DPassThrough_Translucent"));
	if (IsValid(ShieldWidget) && IsValid(ShieldWidgetClassMat))
	{
		ShieldWidget->SetMaterial(0, ShieldWidgetClassMat);
	}
	
	UpdateMoveType(EMoveType::Jogging);
	SetLinkAnimClassLayer(EEquipWeapon::UnArmed);

	HealthComponent->UpdateHealthUI();
	HealthComponent->UpdateShieldUI();

	CombatComponent->UpdateWeaponWidget(EEquipWeapon::Pistol,false);
	CombatComponent->UpdateWeaponWidget(EEquipWeapon::Rifle, false);
	CombatComponent->SetWeaponWidgetVisibility(EEquipWeapon::Pistol, false);
	CombatComponent->SetWeaponWidgetVisibility(EEquipWeapon::Rifle, false);

	if (IsValid(AimCurve.Get()))
	{
		AimTimeline = FTimeline();
		AimTimeline.AddInterpFloat(AimCurve.Get(), AimInterpFunction, FName("Alpha"));
		AimTimeline.SetTimelineFinishedFunc(AimTimeLineFinished);
		AimTimeline.SetLooping(false);		
	}
}

void ALeCharacter::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	AimTimeline.TickTimeline(InDeltaTime);

	if (IAnimInterface* Interface = Cast<IAnimInterface>(GetMesh()->GetAnimInstance()))
	{
		if (const float Dist = GetGroundDistance(); Dist >= 0.0f)
		{
			Interface->ReceiveGroundDistance(Dist);
		}
	}

	if (HasAuthority())
	{
		ReplicatedAcceleration = GetCharacterMovement()->GetCurrentAcceleration();
	}

	if (IAnimInterface* Interface = Cast<IAnimInterface>(GetMesh()->GetAnimInstance()))
	{
		Interface->ReceiveReplicatedAcceleration(ReplicatedAcceleration);
	}
}

void ALeCharacter::SetupPlayerInputComponent(UInputComponent* InPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(InPlayerInputComponent);
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = UCharacterHelper::GetInputLocalPlayerSubsystem(Controller))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(CharacterIMC, 100);
	}
	
	UEnhancedInputComponent* PlayerInputComponent = CastChecked<UEnhancedInputComponent>(InPlayerInputComponent);

	PlayerInputComponent->BindAction(SwitchWeaponIA, ETriggerEvent::Triggered, this, &ALeCharacter::Input_SwitchWeapon);
	PlayerInputComponent->BindAction(SelectWeapon1IA,ETriggerEvent::Triggered, this, &ALeCharacter::Input_SelectWeapon1);
	PlayerInputComponent->BindAction(SelectWeapon2IA,ETriggerEvent::Triggered, this, &ALeCharacter::Input_SelectWeapon2);
	PlayerInputComponent->BindAction(SelectWeapon3IA,ETriggerEvent::Triggered, this, &ALeCharacter::Input_SelectWeapon3);
	PlayerInputComponent->BindAction(LookIA,         ETriggerEvent::Triggered, this, &ALeCharacter::Input_Look);
	PlayerInputComponent->BindAction(MoveIA,         ETriggerEvent::Triggered, this, &ALeCharacter::Input_Move);
	PlayerInputComponent->BindAction(AimIA,          ETriggerEvent::Started,   this, &ALeCharacter::Input_AimStart);
	PlayerInputComponent->BindAction(AimIA,          ETriggerEvent::Completed, this, &ALeCharacter::Input_AimEnd);
	PlayerInputComponent->BindAction(CrouchIA,       ETriggerEvent::Started,   this, &ALeCharacter::Input_Crouch);
	PlayerInputComponent->BindAction(JumpIA,         ETriggerEvent::Started,   this, &ALeCharacter::Input_JumpStart);
	PlayerInputComponent->BindAction(JumpIA,         ETriggerEvent::Completed, this, &ALeCharacter::Input_JumpEnd);
	PlayerInputComponent->BindAction(FireIA,         ETriggerEvent::Triggered, this, &ALeCharacter::Input_Fire);
	PlayerInputComponent->BindAction(InteractionIA,  ETriggerEvent::Started,   this, &ALeCharacter::Input_Interaction);
	PlayerInputComponent->BindAction(ReloadIA,       ETriggerEvent::Started,   this, &ALeCharacter::Input_Reload);
	PlayerInputComponent->BindAction(MainMenuIA,     ETriggerEvent::Triggered, this, &ALeCharacter::Input_MainMenu);
}

#pragma region - CharacterInter Interface
bool ALeCharacter::DoDamage_Implementation(FName InHitBoneName, AActor* InDamageInstigator)
{
	if (HasAuthority())
	{
		constexpr float CalculatedDamageByBoneName = 10.0f;
        LOG_NET_FUNC_FLOAT("DoDamage", CalculatedDamageByBoneName);
        Execute_DecreaseHealth(this, CalculatedDamageByBoneName);
	}
	
	return true;
}

void ALeCharacter::IncreaseHealth_Implementation(float InAmount)
{
	HealthComponent->IncreaseHealth(InAmount);
}

void ALeCharacter::DecreaseHealth_Implementation(float InAmount)
{
	HealthComponent->DecreaseHealth(InAmount);
}

void ALeCharacter::IncreaseShield_Implementation(float InAmount)
{
	HealthComponent->IncreaseShield(InAmount);
}

void ALeCharacter::IncreasePistolClip_Implementation(int InAmount)
{
	CombatComponent->IncreaseClip(EEquipWeapon::Pistol, InAmount);
}

void ALeCharacter::IncreaseRifleClip_Implementation(int InAmount)
{
	CombatComponent->IncreaseClip(EEquipWeapon::Rifle, InAmount);
}
#pragma endregion

float ALeCharacter::GetGroundDistance() const
{
	float GroundDistance = -1.0f;
	if ( IsValid(GetCapsuleComponent()) )
	{
		FHitResult OutHit;
		
		const FVector TraceStartLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		const FVector TraceEndLocation   = GetActorLocation() - FVector(0,0,1000);
		constexpr float Radius = 5.0f;
		const     ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
		constexpr bool TraceComplex = false;
		const     TArray<AActor*> ActorsToIgnore = TArray<AActor*>{};
		constexpr EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
		constexpr bool IgnoreSelf = true;
		const     FLinearColor TraceColor    = FLinearColor::Red;
		const     FLinearColor TraceHitColor = FLinearColor::Green;
		constexpr float DrawTime = 1.0f;
		
		if ( UKismetSystemLibrary::SphereTraceSingle(this, TraceStartLocation, TraceEndLocation,
			Radius, TraceChannel, TraceComplex, ActorsToIgnore, DrawDebugType, OutHit, IgnoreSelf, TraceColor, TraceHitColor, DrawTime) )
		{
			GroundDistance = OutHit.Distance;
		}
	}
	return GroundDistance;
}

void ALeCharacter::UpdateMoveType(const EMoveType InMoveType)
{
	CurrentMoveType = InMoveType;
	
	if (IsLocallyControlled())
	{
		Server_UpdateMoveType(CurrentMoveType);
	}

	if (IAnimInterface* Interface = Cast<IAnimInterface>(GetMesh()->GetAnimInstance()))
	{
		Interface->ReceiveCurrentMoveType(CurrentMoveType);
	}

	if (FWeaponMovementSettings Settings; FWeaponMovementSettings::GetMovementSetting(WeaponMovementSettingDataTable, EquipWeapon, CurrentMoveType, Settings))
	{
		if (UCharacterMovementComponent* Movement = GetCharacterMovement())
		{
			Movement->MaxWalkSpeed                = Settings.MaxWalkSpeed;
			Movement->MaxAcceleration             = Settings.MaxAcceleration;
			Movement->BrakingDecelerationWalking  = Settings.BrakingDeceleration;
			Movement->BrakingFrictionFactor       = Settings.BrakingFrictionFactor;
			Movement->BrakingFriction             = Settings.BrakingFriction;
			Movement->bUseSeparateBrakingFriction = Settings.UseSeperateBrakingFriction;

			if (EMoveType::Crouching == InMoveType)
			{
				Movement->MaxWalkSpeedCrouched = Settings.MaxWalkSpeed;
			}
		}
	}
}

void ALeCharacter::Server_UpdateMoveType_Implementation(const EMoveType InMoveType)
{
	// CurrentMoveType = InMoveType;
}

void ALeCharacter::OnRep_MoveType()
{
	UpdateMoveType(CurrentMoveType);
}

void ALeCharacter::OnRep_Aiming()
{
	bIsAiming ? AimStart() : AimEnd();
}

void ALeCharacter::AimStart()
{
	switch (CurrentMoveType)
	{
	case EMoveType::Walking:   UpdateMoveType(EMoveType::Walking);   break;
	case EMoveType::Jogging:   UpdateMoveType(EMoveType::Walking);   break;
	case EMoveType::Crouching: UpdateMoveType(EMoveType::Crouching); break;
	default: break;
	}
	
	AimTimeline.Play();
}

void ALeCharacter::AimEnd()
{
	switch (CurrentMoveType)
	{
	case EMoveType::Walking:   UpdateMoveType(EMoveType::Jogging);   break;
	case EMoveType::Jogging:   UpdateMoveType(EMoveType::Jogging);   break;
	case EMoveType::Crouching: UpdateMoveType(EMoveType::Crouching); break;
	default: break;
	}

	AimTimeline.Reverse();
}

void ALeCharacter::Server_DoParkour_Implementation()
{
	Multicast_DoParkour();
}

void ALeCharacter::Multicast_DoParkour_Implementation()
{
	if (IsLocallyControlled()) return;
		Parkour->DoParkour();
}

void ALeCharacter::Process_SelectWeapon(const EEquipWeapon InWeapon)
{
	ChangeWeapon(InWeapon);

	PistolWidget->SetVisibility(false);
	RifleWidget->SetVisibility(false);

	switch (InWeapon)
	{
	case EEquipWeapon::Pistol:
		PistolWidget->SetVisibility(true);
		break;
	case EEquipWeapon::Rifle:
		RifleWidget->SetVisibility(true);
		break;
	default: break;
	}
	
	CombatComponent->ProcessWidgetByAnim();
}

void ALeCharacter::ChangeWeapon(const EEquipWeapon InWeapon)
{
	EquipWeapon = InWeapon;
	
	OnWeaponChanged.Broadcast(InWeapon);

	if (IsLocallyControlled())
	{
		Server_ChangeWeapon(EquipWeapon);
	}
	
	if (!IsValid(GetMesh())) return;
	if (!IsValid(Pistol) || !IsValid(Rifle)) return;
	
	switch (InWeapon)
	{
		case EEquipWeapon::UnArmed:
			{
				Pistol->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketNames.PistolUnEquipe);
				Rifle->AttachToComponent (GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketNames.RifleUnEquipe);
			}
			break;
		case EEquipWeapon::Pistol:
			{
				Pistol->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketNames.WeaponEquiped);
				Rifle->AttachToComponent (GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketNames.RifleUnEquipe);
			}
		 break;
		case EEquipWeapon::Rifle:
			{
				Pistol->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketNames.PistolUnEquipe);
				Rifle->AttachToComponent (GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketNames.WeaponEquiped);
			}
			break;
		default: break;
	}
	
	SetLinkAnimClassLayer(InWeapon);
}

void ALeCharacter::Server_ChangeWeapon_Implementation(const EEquipWeapon InWeapon)
{
	EquipWeapon = InWeapon;
}

void ALeCharacter::OnRep_EquipWeapon()
{
	ChangeWeapon(EquipWeapon);
}

void ALeCharacter::SetLinkAnimClassLayer(const EEquipWeapon InWeapon) const
{
	if (!IsValid(GetMesh())) return;
	
	switch (InWeapon)
	{
	case EEquipWeapon::UnArmed: GetMesh()->LinkAnimClassLayers(UnArmedAnimClass); break;
	case EEquipWeapon::Pistol:  GetMesh()->LinkAnimClassLayers(PistolAnimClass);  break;
	case EEquipWeapon::Rifle:   GetMesh()->LinkAnimClassLayers(RifleAnimClass);   break;
	default:
		LOG_ERROR("Unknown weapon type!");
		break;
	}
}

void ALeCharacter::OnAimTimelineFloatReturn(const float InValue)
{
	if (IsValid(SpringArm))
	{
		SpringArm->TargetArmLength = FMath::Lerp(350.0f, 250.0f, InValue);
	}
}

void ALeCharacter::OnAimTimelineFinished()
{
	CombatComponent->ProcessWidgetByAnim();
}

void ALeCharacter::Input_SwitchWeapon()
{
	Process_SelectWeapon(ToggleEnum(EquipWeapon));
}

void ALeCharacter::Input_SelectWeapon1()
{
	Process_SelectWeapon(EEquipWeapon::UnArmed);
}

void ALeCharacter::Input_SelectWeapon2()
{
	Process_SelectWeapon(EEquipWeapon::Pistol);
}

void ALeCharacter::Input_SelectWeapon3()
{
	Process_SelectWeapon(EEquipWeapon::Rifle);
}

void ALeCharacter::Input_Look(const FInputActionValue& InValue)
{
	AddControllerYawInput  (InValue.Get<FVector2D>().X);
	AddControllerPitchInput(InValue.Get<FVector2D>().Y);

	// LOG_INFO("Input_Look: X=%.1f, Y=%.1f", InValue.Get<FVector2D>().X, InValue.Get<FVector2D>().Y);
}

void ALeCharacter::Input_Move(const FInputActionValue& InValue)
{
	const FRotator RightControlRotation(GetControlRotation().Pitch, GetControlRotation().Yaw, 0);
	const FRotator ForwardControlRotation(0, GetControlRotation().Yaw, 0);
	
	const FVector RightMoveDir   = UKismetMathLibrary::GetRightVector  (RightControlRotation);
	const FVector ForwardMoveDir = UKismetMathLibrary::GetForwardVector(ForwardControlRotation);
	
	AddMovementInput(FVector(RightMoveDir),   InValue.Get<FVector2D>().X);
	AddMovementInput(FVector(ForwardMoveDir), InValue.Get<FVector2D>().Y);

	// LOG_INFO("InputScale: X=%.1f, Y=%.1f", InValue.Get<FVector2D>().X, InValue.Get<FVector2D>().Y);
}

void ALeCharacter::Input_AimStart()
{
	SetIsAiming(true);
	AimStart();
	Server_SetIsAiming(true);
}

void ALeCharacter::Input_AimEnd()
{
	SetIsAiming(false);
	AimEnd();
	Server_SetIsAiming(false);
}

void ALeCharacter::Input_Crouch()
{
	switch (CurrentMoveType)
	{
	case EMoveType::Walking:
	case EMoveType::Jogging:
		UpdateMoveType(EMoveType::Crouching);
		Crouch();
		break;
	case EMoveType::Crouching:
		UpdateMoveType(EMoveType::Jogging);
		UnCrouch();
		break;
		default: break;
	}
}

void ALeCharacter::Input_JumpStart()
{
	Parkour->DoParkour();

	switch (Parkour->GetParkourAction())
	{
		case EParkour::Climb:
		case EParkour::Mantle:
		case EParkour::Vault:
		{
			ChangeWeapon(EEquipWeapon::UnArmed);				
		}
		break;
		default:
		{
			Jump();
		}
		break;
	}

	if (GetNetMode() == NM_Standalone) return;
		Server_DoParkour();
}

void ALeCharacter::Input_JumpEnd()
{
	StopJumping();
}

void ALeCharacter::Input_Fire()
{
	CombatComponent->WeaponFire(Camera->GetComponentLocation(), Camera->GetForwardVector());
	
	if (GetNetMode() == NM_Standalone) return;
	CombatComponent->Server_WeaponFire(Camera->GetComponentLocation(), Camera->GetForwardVector());
}

void ALeCharacter::Input_Reload()
{
	CombatComponent->Reload();
	
	if (GetNetMode() == NM_Standalone) return;
	CombatComponent->Server_Reload();
}

void ALeCharacter::Input_Interaction()
{
	// if (!bCanInteract) return;
}

void ALeCharacter::Input_MainMenu()
{
	LOG_INFO("Input_MainMenu");
	
	ALePlayerController* PC = Cast<ALePlayerController>(GetController());
	if (PC && PC->IsLocalController())
	{
		PC->ToggleMainMenu();
	}
}
