// Fill out your copyright notice in the Description page of Project Settings.

#include "LeCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ParkourComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CombatComponent.h"
#include "Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Legacy/Legacy.h"
#include "Legacy/Helpers/CharacterHelper.h"
#include "Legacy/Interfaces/AnimInterface.h"
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

	if (IsValid(GetCharacterMovement()))
	{
		GetCharacterMovement()->SetCrouchedHalfHeight(60.0f);
		GetCharacterMovement()->MaxWalkSpeedCrouched = 250.f;
		GetCharacterMovement()->JumpZVelocity = 700.0f;
		GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
		GetCharacterMovement()->AirControl = 0.35f;
	}
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	if (ConcreteImpactSounds.IsEmpty())
	{
		static ConstructorHelpers::FObjectFinder<USoundBase> ConcreteImpactSound1Asset(TEXT("/Game/ALS/MetaSound/MSP_ImpactPlaster"));
		if (ConcreteImpactSound1Asset.Succeeded()) { ConcreteImpactSounds.Add(ConcreteImpactSound1Asset.Object); }
		
		static ConstructorHelpers::FObjectFinder<USoundBase> ConcreteImpactSound2Asset(TEXT("/Game/ALS/MetaSound/MSP_ImpactPlasterDebries"));
		if (ConcreteImpactSound1Asset.Succeeded()) { ConcreteImpactSounds.Add(ConcreteImpactSound1Asset.Object); }
	}

	if (GlassImpactSounds.IsEmpty())
	{
		static ConstructorHelpers::FObjectFinder<USoundBase> ConcreteImpactSound1Asset(TEXT("/Game/ALS/MetaSound/MSP_ImpactGlass"));
		if (ConcreteImpactSound1Asset.Succeeded()) { GlassImpactSounds.Add(ConcreteImpactSound1Asset.Object); }
		
		static ConstructorHelpers::FObjectFinder<USoundBase> ConcreteImpactSound2Asset(TEXT("/Game/ALS/MetaSound/MSP_ImpactGlassDebries"));
		if (ConcreteImpactSound1Asset.Succeeded()) { GlassImpactSounds.Add(ConcreteImpactSound1Asset.Object); }
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FireTracerEffectAsset(TEXT("/Game/ALS/Effects/Particles/Weapons/NS_WeaponFire_Tracer"));
	if (FireTracerEffectAsset.Succeeded()) { FireTracerEffect = FireTracerEffectAsset.Object; }

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ConcreteImpactEffectAsset(TEXT("/Game/ALS/Effects/Particles/Impacts/NS_ImpactConcrete"));
	if (ConcreteImpactEffectAsset.Succeeded()) { ConcreteImpactEffect = ConcreteImpactEffectAsset.Object; }
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> GlassImpactEffectAsset(TEXT("/Game/ALS/Effects/Particles/Impacts/NS_ImpactGlass"));
	if (GlassImpactEffectAsset.Succeeded()) { GlassImpactEffect = GlassImpactEffectAsset.Object; }

	static ConstructorHelpers::FObjectFinder<UCurveFloat> AimCurveAsset(TEXT("/Game/TP/Curves/CF_Aim"));
	if (AimCurveAsset.Succeeded()) { AimCurve = AimCurveAsset.Object; }
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PistolFireMontageAsset(TEXT("/Game/ALS/Animations/Pistol/Gun/AM_MM_Pistol_Fire"));
	if (PistolFireMontageAsset.Succeeded()) { PistolFireMontage = PistolFireMontageAsset.Object; }
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PistolReloadMontageAsset(TEXT("/Game/ALS/Animations/Pistol/Gun/AM_MM_Pistol_Reload"));
	if (PistolReloadMontageAsset.Succeeded()) { PistolReloadMontage = PistolReloadMontageAsset.Object; }

	static ConstructorHelpers::FObjectFinder<UAnimSequence> PistolFireAnimSequenceAsset(TEXT("/Game/ALS/Animations/Pistol/Gun/Weap_Pistol_Fire"));
	if (PistolFireAnimSequenceAsset.Succeeded()) { PistolFireAnimSequence = PistolFireAnimSequenceAsset.Object; }

	static ConstructorHelpers::FObjectFinder<UAnimSequence> PistolReloadAnimSequenceAsset(TEXT("/Game/ALS/Animations/Pistol/Gun/Weap_Pistol_Reload"));
	if (PistolReloadAnimSequenceAsset.Succeeded()) { PistolReloadAnimSequence = PistolReloadAnimSequenceAsset.Object; }

	static ConstructorHelpers::FObjectFinder<USoundBase> PistolFireSoundAsset(TEXT("/Game/ALS/MetaSound/MSP_PistolFire"));
	if (PistolFireSoundAsset.Succeeded()) { PistolFireSound = PistolFireSoundAsset.Object; }
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RifleFireMontageAsset(TEXT("/Game/ALS/Animations/Rifle/Gun/AM_MM_Rifle_Fire"));
	if (RifleFireMontageAsset.Succeeded()) { RifleFireMontage = RifleFireMontageAsset.Object; }
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RifleReloadMontageAsset(TEXT("/Game/ALS/Animations/Rifle/Gun/AM_MM_Rifle_Reload"));
	if (RifleReloadMontageAsset.Succeeded()) { RifleReloadMontage = RifleReloadMontageAsset.Object; }

	static ConstructorHelpers::FObjectFinder<UAnimSequence> RifleFireAnimSequenceAsset(TEXT("/Game/ALS/Animations/Rifle/Gun/Weap_Rifle_Fire"));
	if (RifleFireAnimSequenceAsset.Succeeded()) { RifleFireAnimSequence = RifleFireAnimSequenceAsset.Object; }

	static ConstructorHelpers::FObjectFinder<UAnimSequence> RifleReloadAnimSequenceAsset(TEXT("/Game/ALS/Animations/Rifle/Gun/Weap_Rifle_Reload"));
	if (RifleReloadAnimSequenceAsset.Succeeded()) { RifleReloadAnimSequence = RifleReloadAnimSequenceAsset.Object; }

	static ConstructorHelpers::FObjectFinder<USoundBase> RifleFireSoundAsset(TEXT("/Game/ALS/MetaSound/MSP_RifleFire"));
	if (RifleFireSoundAsset.Succeeded()) { RifleFireSound = RifleFireSoundAsset.Object; }

	if (IsValid(GetMesh()))
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodySkeletalAsset(TEXT("/Game/ALS/Characters/Heroes/Mannequin/Meshes/SKM_Manny"));
		if (BodySkeletalAsset.Succeeded()) { GetMesh()->SetSkeletalMesh(BodySkeletalAsset.Object); }
		
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
		
		Torch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Torch"));
		Torch->SetupAttachment(GetMesh(), FName("Torch"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> TorchAsset(TEXT("/Game/ALS/UI/StaticMeshes/Torch/SM_Torch"));
		if (TorchAsset.Succeeded()) { Torch->SetStaticMesh(TorchAsset.Object); }
	
		TorchHolder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TorchHolder"));
		TorchHolder->SetupAttachment(GetMesh(), FName("TorchHolder"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> TorchHolderAsset(TEXT("/Game/ALS/UI/StaticMeshes/Torch/SM_TorchHolder"));
		if (TorchHolderAsset.Succeeded()) { TorchHolder->SetStaticMesh(TorchHolderAsset.Object); }

		PistolHolster = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PistolHolster"));
		PistolHolster->SetupAttachment(GetMesh(), FName("PistolHolster"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> PistolHolsterAsset(TEXT("/Game/ALS/UI/StaticMeshes/Holster/SM_Holster_R"));
		if (PistolHolsterAsset.Succeeded()) { PistolHolster->SetStaticMesh(PistolHolsterAsset.Object); }

		Helmet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Helmet"));
		Helmet->SetupAttachment(GetMesh(), FName("Helmet"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> HelmetAsset(TEXT("/Game/ALS/UI/StaticMeshes/Helmet/SM_Manny_Helmet"));
		if (HelmetAsset.Succeeded()) { Helmet->SetStaticMesh(HelmetAsset.Object); }

		HealthBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthBar"));
		HealthBar->SetupAttachment(GetMesh(), FName("HealthBar"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> HealthBarAsset(TEXT("/Game/ALS/UI/StaticMeshes/HealthBar/SM_BodyHealthBar"));
		if (HealthBarAsset.Succeeded()) { HealthBar->SetStaticMesh(HealthBarAsset.Object); }

		Pistol = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pistol"));
		Pistol->SetupAttachment(GetMesh(), FName("PistolUnEquipe"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> PistolAsset(TEXT("/Game/ALS/SkeletalMeshs/Pistol/SK_Pistol"));
		if (PistolAsset.Succeeded()) { Pistol->SetSkeletalMesh(PistolAsset.Object); }
		
		Rifle = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Rifle"));
		Rifle->SetupAttachment(GetMesh(), FName("RifleUnEquipe"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> RifleAsset(TEXT("/Game/ALS/SkeletalMeshs/Rifle/SK_Rifle"));
		if (RifleAsset.Succeeded()) { Rifle->SetSkeletalMesh(RifleAsset.Object); }
	}

	if (IsValid(Pistol))
	{
		PistolWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PistolWidget"));
		PistolWidget->SetupAttachment(Pistol, FName("Widget"));
		PistolWidget->SetWidgetSpace(EWidgetSpace::Screen);
		PistolWidget->SetDrawSize(FVector2D(400, 80));
		static ConstructorHelpers::FClassFinder<UUserWidget> PistolWidgetAsset(TEXT("/Game/ALS/UI/Widgets/WBP_PistolUI"));
		if (PistolWidgetAsset.Succeeded()) { PistolWidget->SetWidgetClass(PistolWidgetAsset.Class); }
	}
	
	if (IsValid(Rifle))
	{
		RifleWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("RifleWidget"));
		RifleWidget->SetupAttachment(Rifle, FName("Widget"));
		RifleWidget->SetWidgetSpace(EWidgetSpace::Screen);
		RifleWidget->SetDrawSize(FVector2D(400, 80));
		static ConstructorHelpers::FClassFinder<UUserWidget> RifleWidgetAsset(TEXT("/Game/ALS/UI/Widgets/WBP_RifleUI"));
		if (RifleWidgetAsset.Succeeded()) { RifleWidget->SetWidgetClass(RifleWidgetAsset.Class); }
	}

	if (IsValid(HealthBar))
	{
		ShieldWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ShieldWidget"));
		ShieldWidget->SetupAttachment(HealthBar, FName("None"));
		ShieldWidget->SetDrawSize(FVector2D(50, 50));
		ShieldWidget->SetRelativeLocation(FVector(-2.6f, -1.5f, 19.1f));
		ShieldWidget->SetRelativeRotation(FRotator(0, -90, 0));
		ShieldWidget->SetRelativeScale3D(FVector(1.0f, 0.09f, 0.09f));
	}
	
	static ConstructorHelpers::FClassFinder<UUserWidget> ViewPortCrossHairWidgetAsset(TEXT("/Game/ALS/UI/Widgets/WBP_CrossHair"));
	if (ViewPortCrossHairWidgetAsset.Succeeded()) { ViewPortCrossHairWidgetClass = ViewPortCrossHairWidgetAsset.Class; }

	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponMovementSettingAsset(TEXT("/Game/TP/DataTables/DT_WeaponMovement"));
	if (WeaponMovementSettingAsset.Succeeded()) { WeaponMovementSettingDataTable = WeaponMovementSettingAsset.Object; }
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> UnArmedAnimClassBP(TEXT("/Game/TP/ABP_TpUnArmed"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> PistolAnimClassBP (TEXT("/Game/TP/ABP_TpPistol"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> RifleAnimClassBP  (TEXT("/Game/TP/ABP_TpRifle"));
	if (UnArmedAnimClassBP.Class != nullptr) { UnArmedAnimClass = UnArmedAnimClassBP.Class; }
	if (PistolAnimClassBP.Class  != nullptr) { PistolAnimClass  = PistolAnimClassBP.Class; }
	if (RifleAnimClassBP.Class   != nullptr) { RifleAnimClass   = RifleAnimClassBP.Class; }
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> CharacterIMCAsset(TEXT("/Game/ALS/Inputs/IMC_ALS"));
	if (CharacterIMCAsset.Succeeded())
	{
		CharacterIMC = CharacterIMCAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> AimInputActionAsset(TEXT("/Game/ALS/Inputs/InputActions/IA_Aim"));
	static ConstructorHelpers::FObjectFinder<UInputAction> CrouchAsset        (TEXT("/Game/ALS/Inputs/InputActions/IA_Crouch"));
	static ConstructorHelpers::FObjectFinder<UInputAction> FireAsset          (TEXT("/Game/ALS/Inputs/InputActions/IA_Fire"));
	static ConstructorHelpers::FObjectFinder<UInputAction> InteractionAsset   (TEXT("/Game/ALS/Inputs/InputActions/IA_Interaction"));
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpAsset          (TEXT("/Game/ALS/Inputs/InputActions/IA_Jump"));
	static ConstructorHelpers::FObjectFinder<UInputAction> LookAsset          (TEXT("/Game/ALS/Inputs/InputActions/IA_Look"));
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveAsset          (TEXT("/Game/ALS/Inputs/InputActions/IA_Move"));
	static ConstructorHelpers::FObjectFinder<UInputAction> ReloadAsset        (TEXT("/Game/ALS/Inputs/InputActions/IA_Reload"));
	static ConstructorHelpers::FObjectFinder<UInputAction> SwitchWeaponAsset  (TEXT("/Game/ALS/Inputs/InputActions/IA_SwitchWeapon"));
	
	if (AimInputActionAsset.Succeeded()) { AimIA          = AimInputActionAsset.Object; }
	if (CrouchAsset.Succeeded())         { CrouchIA       = CrouchAsset.Object; }
	if (FireAsset.Succeeded())           { FireIA         = FireAsset.Object; }
	if (InteractionAsset.Succeeded())    { InteractionIA  = InteractionAsset.Object; }
	if (JumpAsset.Succeeded())           { JumpIA         = JumpAsset.Object; }
	if (LookAsset.Succeeded())           { LookIA         = LookAsset.Object; }
	if (MoveAsset.Succeeded())           { MoveIA         = MoveAsset.Object; }
	if (ReloadAsset.Succeeded())         { ReloadIA       = ReloadAsset.Object; }
	if (SwitchWeaponAsset.Succeeded())   { SwitchWeaponIA = SwitchWeaponAsset.Object; }

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
		Subsystem->AddMappingContext(CharacterIMC, 0);
		
		ViewPortCrossHairWidget = CreateWidget(Cast<APlayerController>(Controller), ViewPortCrossHairWidgetClass);
		ViewPortCrossHairWidget->AddToViewport();
	}
	else
	{
		if (IsValid(ViewPortCrossHairWidget))
		{
			ViewPortCrossHairWidget->RemoveFromParent();
			ViewPortCrossHairWidget->Destruct();
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
	UClass* ShieldWidgetClass = LoadObject<UClass>(nullptr, TEXT("/Game/ALS/UI/Widgets/WBP_Shield.WBP_Shield_C"));
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
}

void ALeCharacter::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	AimTimeline.TickTimeline(InDeltaTime);

	if (IAnimInterface* Interface = Cast<IAnimInterface>(GetMesh()->GetAnimInstance()))
	{
		if ( 0.0f <= GetGroundDistance())
		{
			Interface->ReceiveGroundDistance(GetGroundDistance());
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
		Subsystem->AddMappingContext(CharacterIMC, 0);
	}
	
	UEnhancedInputComponent* PlayerInputComponent = CastChecked<UEnhancedInputComponent>(InPlayerInputComponent);

	PlayerInputComponent->BindAction(SwitchWeaponIA, ETriggerEvent::Triggered, this, &ALeCharacter::Input_SwitchWeapon);
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
	
	if (IsValid(AimCurve))
	{
		AimTimeline.AddInterpFloat(AimCurve, AimInterpFunction, FName("Alpha"));
		AimTimeline.SetTimelineFinishedFunc(AimTimeLineFinished);
		AimTimeline.SetLooping(false);
		AimTimeline.Play();
	}
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

	if (IsValid(AimCurve))
	{
		AimTimeline.AddInterpFloat(AimCurve, AimInterpFunction, FName("Alpha"));
		AimTimeline.SetTimelineFinishedFunc(AimTimeLineFinished);
		AimTimeline.SetLooping(false);
		AimTimeline.Reverse();
	}
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

void ALeCharacter::Input_SwitchWeapon(const FInputActionValue& InValue)
{
	if (const int32 InputValue = static_cast<int32>(InValue.Get<float>()))
	{
		ChangeWeapon(static_cast<EEquipWeapon>(InputValue - 1));
	}

	UpdateMoveType(CurrentMoveType);

	PistolWidget->SetVisibility(false);
	RifleWidget->SetVisibility(false);
	
	CombatComponent->ProcessWidgetByAnim();
}

void ALeCharacter::Input_Look(const FInputActionValue& InValue)
{
	AddControllerYawInput  (InValue.Get<FVector2D>().X);
	AddControllerPitchInput(InValue.Get<FVector2D>().Y);
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
