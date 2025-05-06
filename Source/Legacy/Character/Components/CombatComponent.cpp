// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "Legacy/Legacy.h"
#include "Legacy/Character/LeCharacter.h"
#include "Legacy/Interfaces/CharacterInterface.h"
#include "Legacy/Interfaces/WidgetInterface.h"
#include "Legacy/SubSystems/AsyncSpawnSubsystem.h"
#include "Legacy/Types/LeTypes.h"

UCombatComponent::UCombatComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
	
	bCanFire = true;

	bUseDebugFireTrace = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeWeaponData();
}

void UCombatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	LeCharacter = Cast<ALeCharacter>(GetOwner());
	LeCharacter->OnWeaponChanged.AddDynamic(this, &UCombatComponent::HandleWeaponChanged);
}

void UCombatComponent::InitializeWeaponData()
{
	FWeaponData PistolData;
	PistolData.ClipSize = 5; PistolData.ClipAmount  = 4; PistolData.BulletAmount = 3; PistolData.MaxClipAmount = 10;
	PistolData.FireDelay = 0.5f;
	PistolData.FireTraceStartSocketName = FName("Muzzle");
	PistolData.FireMontage        = LeCharacter->GetPistolFireMontage();
	PistolData.FireAnimSequence   = LeCharacter->GetPistolFireAnimSequence();
	PistolData.FireSound          = LeCharacter->GetPistolFireSound();
	PistolData.ReloadMontage      = LeCharacter->GetPistolReloadMontage();
	PistolData.ReloadAnimSequence = LeCharacter->GetPistolReloadAnimSequence();
	PistolData.Widget             = LeCharacter->GetPistolWidget();
	PistolData.SkeletalMesh       = LeCharacter->GetPistolSkeletalMesh();
	WeaponDataMap.Add(EEquipWeapon::Pistol, PistolData);

	FWeaponData RifleData;
	RifleData.ClipSize = 10; RifleData.ClipAmount = 2; RifleData.BulletAmount = 5; RifleData.MaxClipAmount = 10;
	RifleData.FireDelay = 0.2f;
	RifleData.FireTraceStartSocketName = FName("Muzzle");
	RifleData.FireMontage        = LeCharacter->GetRifleFireMontage();
	RifleData.FireAnimSequence   = LeCharacter->GetRifleFireAnimSequence();
	RifleData.FireSound          = LeCharacter->GetRifleFireSound();
	RifleData.ReloadMontage      = LeCharacter->GetRifleReloadMontage();
	RifleData.ReloadAnimSequence = LeCharacter->GetRifleReloadAnimSequence();
	RifleData.Widget             = LeCharacter->GetRifleWidget();
	RifleData.SkeletalMesh       = LeCharacter->GetRifleSkeletalMesh();
	WeaponDataMap.Add(EEquipWeapon::Rifle, RifleData);
	
	ForEachEnum<EEquipWeapon>([this](const EEquipWeapon InWeaponType)
	{
		if (WeaponDataMap.Find(InWeaponType))
			checkf(WeaponDataMap.Find(InWeaponType), TEXT("WeaponData by weapon type must always be paired."));
	});
}

FWeaponData* UCombatComponent::GetCurrentWeaponData()
{
	return WeaponDataMap.Find(LeCharacter->GetCurrentEquipWeapon());
}

void UCombatComponent::HandleWeaponChanged(EEquipWeapon NewWeapon)
{
	bCanFire = true;
}

void UCombatComponent::WeaponFire(const FVector& InCameraLoc = FVector::ZeroVector, const FVector& InCameraForward = FVector::ZeroVector)
{
	if (!IsValid(GetWorld()) || !IsValid(LeCharacter->GetMesh()) || !IsValid(LeCharacter->GetMesh()->GetAnimInstance())) return;
	
	if (EMoveType::Jogging    == LeCharacter->GetCurrentMoveType()) return;
	if (EEquipWeapon::UnArmed == LeCharacter->GetCurrentEquipWeapon()) return;
	if (!bCanFire) return;

	FWeaponData* WeaponData = WeaponDataMap.Find(LeCharacter->GetCurrentEquipWeapon());
	if (nullptr == WeaponData)
		return;

	if (WeaponData->BulletAmount > 0)
	{
		bCanFire = false;
		WeaponData->BulletAmount--;

		LeCharacter->GetMesh()->GetAnimInstance()->Montage_Play(WeaponData->FireMontage);

		if (IsValid(WeaponData->SkeletalMesh))
		{
			WeaponData->SkeletalMesh->PlayAnimation(WeaponData->FireAnimSequence, false);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponData->FireSound, WeaponData->SkeletalMesh->GetBoneLocation(FName("Barrel")));
		}

		FireLineTrace(InCameraLoc, InCameraForward);

		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, [this]()
		{
			bCanFire = true;
		}, WeaponData->FireDelay, false);

		UpdateWeaponWidget(LeCharacter->GetCurrentEquipWeapon());
	}
	else
	{
		bCanFire = false;
		Reload();
	}
}

void UCombatComponent::FireLineTrace(const FVector& InStartLocation, const FVector& InDirection)
{
	const FWeaponData* WeaponData = WeaponDataMap.Find(LeCharacter->GetCurrentEquipWeapon());
	
	FHitResult HitResult;

	      FVector Start = InStartLocation;
	const FVector End   = Start + (InDirection * 50000.0f);

	if (GetNetMode() == NM_Client && LeCharacter->GetLocalRole() == ROLE_SimulatedProxy)
	{
		Start = WeaponData->SkeletalMesh->GetSocketLocation(WeaponData->FireTraceStartSocketName);
	}

	const bool bIsDedicatedServer = NM_DedicatedServer == GetNetMode();
	
	const     ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	constexpr bool bTraceComplex = false;
	const     TArray<AActor*> ActorsToIgnore;
	constexpr EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
	constexpr bool bIgnoreSelf = true;
	const     FLinearColor TraceColor    = bIsDedicatedServer ? FLinearColor::Red : FLinearColor::Green;
	const     FLinearColor TraceHitColor = bIsDedicatedServer ? FLinearColor::Red : FLinearColor::Green;
	const     float DrawTime = bIsDedicatedServer ? 3.0f : 2.0f;

	if (bUseDebugFireTrace)
	{
		DrawDebugLine(GetWorld(), Start, End, bIsDedicatedServer ? FColor::Red : FColor::Green, false,
				bIsDedicatedServer ? 3.0f : 2.0f, 0, bIsDedicatedServer ? 0.5f : 1.0f);		
	}
	
	if (UKismetSystemLibrary::LineTraceSingle(this, Start, End, TraceChannel, bTraceComplex,
		ActorsToIgnore, DrawDebugType, HitResult, bIgnoreSelf, TraceColor, TraceHitColor, DrawTime))
	{
		if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->Implements<UCharacterInterface>())
		{
			ICharacterInterface::Execute_DoDamage(HitResult.GetActor(), HitResult.BoneName, HitResult.GetActor() );
		}
		
		PlayImpactSound(HitResult.ImpactPoint, HitResult.PhysMaterial.Get());
		SpawnImpactEffect(HitResult.ImpactPoint, HitResult.ImpactNormal, HitResult.PhysMaterial.Get());
		SpawnFireTracer(HitResult.ImpactPoint, WeaponData->SkeletalMesh, WeaponData->FireTraceStartSocketName);
	}
	else
	{
		SpawnFireTracer(End, WeaponData->SkeletalMesh, WeaponData->FireTraceStartSocketName);
	}
}

void UCombatComponent::Server_WeaponFire_Implementation(const FVector& InCameraLoc, const FVector& InCameraForward)
{
	WeaponFire(InCameraLoc, InCameraForward);
	Multicast_WeaponFire(InCameraLoc, InCameraForward);
}

void UCombatComponent::Multicast_WeaponFire_Implementation(const FVector& InCameraLoc, const FVector& InCameraForward)
{
	if (LeCharacter->IsLocallyControlled()) return;
	WeaponFire(InCameraLoc, InCameraForward);
}

void UCombatComponent::Reload()
{
	if (!IsValid(LeCharacter->GetMesh())) return;
	UAnimInstance* AnimInstance = LeCharacter->GetMesh()->GetAnimInstance();
	if (!IsValid(AnimInstance)) return;

	FWeaponData* WeaponData = WeaponDataMap.Find(LeCharacter->GetCurrentEquipWeapon());
	
	if (AnimInstance->Montage_IsPlaying(WeaponData->FireMontage)) return;
	if (WeaponData->ClipAmount < 1) return;

	FOnMontageBlendingOutStarted BlendOutDelegate;
	BlendOutDelegate.BindLambda([this, WeaponData](UAnimMontage* InMontage, bool bInInterrupted)
	{
		bCanFire = true;

		WeaponData->ClipAmount--;
		WeaponData->BulletAmount = WeaponData->ClipSize;
		
		FString Log = FString::Format(TEXT("{0} Reload Complete : ClipAmount {1}, BulletAmount {2}"),
			{ GetEnumNameString(LeCharacter->GetCurrentEquipWeapon()),WeaponData->ClipAmount, WeaponData->BulletAmount });
		LOG_NET_FUNC(Log);

		UpdateWeaponWidget(LeCharacter->GetCurrentEquipWeapon());

		ProcessWidgetByAnim();
	});
	
	AnimInstance->Montage_Play(WeaponData->ReloadMontage);
	AnimInstance->Montage_SetBlendingOutDelegate(BlendOutDelegate, WeaponData->ReloadMontage);
	
	if (IsValid(WeaponData->SkeletalMesh) && IsValid(WeaponData->ReloadAnimSequence))
	{
		WeaponData->SkeletalMesh->PlayAnimation(WeaponData->ReloadAnimSequence, false);
	}
}

void UCombatComponent::Server_Reload_Implementation()
{
	Reload();
	Multicast_Reload();

	// AsyncSpawnSystem Test.
	// for (int32 i = 100; i > 0; --i)
	// {
	// 	FTransform SpawnTransform(FRotator::ZeroRotator, FVector(i * 200, 0, 100));
	// 	GetWorld()->GetSubsystem<UAsyncSpawnSubsystem>()->QueueAsyncSpawn(TSoftClassPtr<ALeCharacter>(ALeCharacter::StaticClass()), SpawnTransform);
	// }
}

void UCombatComponent::Multicast_Reload_Implementation()
{
	if (LeCharacter->IsLocallyControlled()) return;
	Reload();
}

void UCombatComponent::IncreaseClip(const EEquipWeapon InWeapon, const int InAmount)
{
	if (FWeaponData* WeaponData = WeaponDataMap.Find(InWeapon))
	{
		WeaponData->ClipAmount = FMath::Clamp(WeaponData->ClipAmount + InAmount, 0, WeaponData->MaxClipAmount);
		UpdateWeaponWidget(InWeapon);
	}
}

void UCombatComponent::SetWeaponWidgetVisibility(const EEquipWeapon InWeapon, const bool bInVisibility)
{
	if (const FWeaponData* WeaponData = WeaponDataMap.Find(InWeapon))
	{
		WeaponData->Widget->SetVisibility(bInVisibility);
	}
}

void UCombatComponent::UpdateWeaponWidget(const EEquipWeapon InWeapon, const bool bUseOnlyLocalPlayer)
{
	if (bUseOnlyLocalPlayer)
	{
		if (GetNetMode() == NM_Client && LeCharacter->GetLocalRole() == ROLE_SimulatedProxy)
			return;
	}
	
	if (const FWeaponData* WeaponData = WeaponDataMap.Find(InWeapon))
	{
		if (IWidgetInterface* Interface = Cast<IWidgetInterface>(WeaponData->Widget->GetUserWidgetObject()))
		{
			Interface->UpdateBulletAmount(WeaponData->BulletAmount);
			Interface->UpdateClipsAmount(WeaponData->ClipAmount);
		}
	}
}

void UCombatComponent::ProcessWidgetByAnim() const
{
	if (GetNetMode() == NM_Client && LeCharacter->GetLocalRole() == ROLE_SimulatedProxy)
		return;
	
	if (const FWeaponData* WeaponData = WeaponDataMap.Find(LeCharacter->GetCurrentEquipWeapon()))
	{
		WeaponData->Widget->SetVisibility(LeCharacter->GetIsAiming());
	}
}


void UCombatComponent::PlayImpactSound(const FVector& InImpactLocation, const UPhysicalMaterial* InPhysicalMaterial) const
{
	TArray<TObjectPtr<USoundBase>> ImpactSounds{};
	
	if (IsValid(InPhysicalMaterial))
	{
		switch (InPhysicalMaterial->SurfaceType)
		{
		case PhysicalSurfaces::Glass:
			ImpactSounds = LeCharacter->GetGlassImpactSounds();
			break;
		case PhysicalSurfaces::Concrete:
		case PhysicalSurfaces::Character:
		default: ImpactSounds = LeCharacter->GetConcreteImpactSounds();
			break;	
		}
	}
	
	for (const auto& Sound : ImpactSounds)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, InImpactLocation);				
	}
}

void UCombatComponent::SpawnImpactEffect(const FVector& InImpactLocation, const FVector& InImpactNormal, const UPhysicalMaterial* InPhysicalMaterial) const
{
	UNiagaraSystem* ImpactEffect = LeCharacter->GetGlassImpactEffect();
	if (!IsValid(ImpactEffect) || !IsValid(InPhysicalMaterial)) return;
	
	switch (InPhysicalMaterial->SurfaceType)
	{
	case PhysicalSurfaces::Glass:
		ImpactEffect = LeCharacter->GetGlassImpactEffect();
		break;
	case PhysicalSurfaces::Concrete:
	case PhysicalSurfaces::Character:
		ImpactEffect = LeCharacter->GetGlassImpactEffect();
	default: break;
	}

	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, FVector::Zero());
	
	if (!IsValid(NiagaraComponent))
		return;

	TArray<FVector> ImpactLocationArray; ImpactLocationArray.Add(InImpactLocation);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayPosition(NiagaraComponent, FName("User.ImpactPositions"), ImpactLocationArray);
	
	TArray<FVector> ImpactNormalArray; ImpactNormalArray.Add(InImpactNormal);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(NiagaraComponent, FName("User.ImpactNormals"), ImpactNormalArray);
}

void UCombatComponent::SpawnFireTracer(const FVector& InImpactLocation, USceneComponent* InWeaponParentComponent, const FName InSpawnSocketName) const
{
	if (UNiagaraSystem* FireTracerEffect = LeCharacter->GetFireTracerEffect())
	{
		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(FireTracerEffect, InWeaponParentComponent,
		InSpawnSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::KeepRelativeOffset, false);
	
		if (IsValid(NiagaraComponent))
		{
			TArray<FVector> ImpactLocationArray; ImpactLocationArray.Add(InImpactLocation);
			UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(NiagaraComponent, FName("User.ImpactPositions"), ImpactLocationArray);
		}
	}
}
