// Fill out your copyright notice in the Description page of Project Settings.

#include "LeVehicle.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Legacy/Legacy.h"
#include "Legacy/Helpers/CharacterHelper.h"

ALeVehicle::ALeVehicle()
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> VehicleIMCAsset(TEXT("/Game/Vehicle/Blueprint/Input/IMC_Vehicle"));
	if (VehicleIMCAsset.Succeeded())
	{
		VehicleIMC = VehicleIMCAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> BrakeAsset      (TEXT("/Game/Vehicle/Blueprint/Input/InputActions/IA_Brake"));
	static ConstructorHelpers::FObjectFinder<UInputAction> HandBrakeAsset  (TEXT("/Game/Vehicle/Blueprint/Input/InputActions/IA_HandBrake"));
	static ConstructorHelpers::FObjectFinder<UInputAction> LookAroundAsset (TEXT("/Game/Vehicle/Blueprint/Input/InputActions/IA_LookAround"));
	static ConstructorHelpers::FObjectFinder<UInputAction> SteeringAsset   (TEXT("/Game/Vehicle/Blueprint/Input/InputActions/IA_Steering"));
	static ConstructorHelpers::FObjectFinder<UInputAction> ThrottleAsset   (TEXT("/Game/Vehicle/Blueprint/Input/InputActions/IA_Throttle"));
	static ConstructorHelpers::FObjectFinder<UInputAction> VehicleExitAsset(TEXT("/Game/Vehicle/Blueprint/Input/InputActions/IA_VehicleExit"));
	if (BrakeAsset.Succeeded())       { BrakeIA       = BrakeAsset.Object; }
	if (HandBrakeAsset.Succeeded())   { HandBrakeIA   = HandBrakeAsset.Object; }
	if (LookAroundAsset.Succeeded())  { LookAroundIA  = LookAroundAsset.Object; }
	if (SteeringAsset.Succeeded())    { SteeringIA    = SteeringAsset.Object; }
	if (ThrottleAsset.Succeeded())    { ThrottleIA    = ThrottleAsset.Object; }
	if (VehicleExitAsset.Succeeded()) { VehicleExitIA = VehicleExitAsset.Object; }
}

ALeCharacter* ALeVehicle::GetInteractionCharacter() const
{
	return InteractionCharacter;
}

void ALeVehicle::PossessedBy(AController* InNewController)
{
	Super::PossessedBy(InNewController);
}

void ALeVehicle::UnPossessed()
{
	Super::UnPossessed();
}

void ALeVehicle::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = UCharacterHelper::GetInputLocalPlayerSubsystem(Controller))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(VehicleIMC, 0);
	}

	NotifyControllerChangedForBP();
}

void ALeVehicle::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(VehicleInteractionComponentClass))
	{
		VehicleInteractionComponent = NewObject<UVehicleInteractionComponent>(this, VehicleInteractionComponentClass);
		VehicleInteractionComponent->RegisterComponent();
		VehicleInteractionComponent->SetIsReplicated(true);
	}
}

void ALeVehicle::BeginPlay()
{
	Super::BeginPlay();
}

void ALeVehicle::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
}

void ALeVehicle::SetupPlayerInputComponent(UInputComponent* InPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(InPlayerInputComponent);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = UCharacterHelper::GetInputLocalPlayerSubsystem(Controller))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(VehicleIMC, 0);
	}
	
	UEnhancedInputComponent* PlayerInputComponent = CastChecked<UEnhancedInputComponent>(InPlayerInputComponent);

	// InputComponent->BindAction(SwitchWeaponIA, ETriggerEvent::Triggered, this, &ALeCharacter::Input_SwitchWeapon);
}

void ALeVehicle::Input_Brake(const FInputActionValue& InValue)
{
	LOG_FUNC();
}

void ALeVehicle::Input_HandBrake(const FInputActionValue& InValue)
{
	LOG_FUNC();
}

void ALeVehicle::Input_LookAround(const FInputActionValue& InValue)
{
	LOG_FUNC();
}

void ALeVehicle::Input_Steering(const FInputActionValue& InValue)
{
	LOG_FUNC();
}

void ALeVehicle::Input_Throttle(const FInputActionValue& InValue)
{
	LOG_FUNC();
}

void ALeVehicle::Input_VehicleExit()
{
	LOG_FUNC();
}
