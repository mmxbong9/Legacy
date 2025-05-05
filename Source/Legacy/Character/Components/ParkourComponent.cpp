// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkourComponent.h"

#include "Legacy/Character/LeCharacter.h"

UParkourComponent::UParkourComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

	ObstacleDetectionChannel = ObjectTypeQuerys::Parkour;

	ClimbMontage = CreateDefaultSubobject<UAnimMontage>(TEXT("ClimbMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ClimbMontageAsset(TEXT("/Game/Parkour/Animations/AM_Climb"));
	if (ClimbMontageAsset.Succeeded()) { ClimbMontage = ClimbMontageAsset.Object; }

	MantleMontage = CreateDefaultSubobject<UAnimMontage>(TEXT("MantleMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MantleMontageAsset(TEXT("/Game/Parkour/Animations/AM_Mantle"));
	if (MantleMontageAsset.Succeeded()) { MantleMontage = MantleMontageAsset.Object; }

	VaultMontage = CreateDefaultSubobject<UAnimMontage>(TEXT("VaultMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> VaultMontageAsset(TEXT("/Game/Parkour/Animations/AM_Vault"));
	if (VaultMontageAsset.Succeeded()) { ClimbMontage = VaultMontageAsset.Object; }
}

void UParkourComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Character = Cast<ALeCharacter>(GetOwner());
	MotionWarpingComponent = ALeCharacter::FindMotionWarpingComponent(GetOwner());
}
