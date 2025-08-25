#pragma once

#include "CoreMinimal.h"
#include "Legacy/Global/GlobalUtil.h"
#include "LeTypes.generated.h"

class UAnimMontage;
class UAnimSequence;
class USoundBase;
class UWidgetComponent;
class USkeletalMeshComponent;

USTRUCT()
struct FWeaponData
{
	GENERATED_BODY()

	int BulletAmount;
	int ClipAmount;
	int ClipSize;
	int MaxClipAmount;

	float FireDelay;
	float ReloadDelay;
	
	FName FireTraceStartSocketName;
	
	UPROPERTY() TObjectPtr<UAnimMontage>  FireMontage;
	UPROPERTY() TObjectPtr<UAnimSequence> FireAnimSequence;
	UPROPERTY() TObjectPtr<USoundBase>    FireSound;
	
	UPROPERTY() TObjectPtr<UAnimMontage>  ReloadMontage;
	UPROPERTY() TObjectPtr<UAnimSequence> ReloadAnimSequence;

	UPROPERTY() TObjectPtr<UWidgetComponent> Widget;
	UPROPERTY() TObjectPtr<USkeletalMeshComponent> SkeletalMesh;
};

UENUM(BlueprintType)
enum class EParkour : uint8
{
	None   = 0 UMETA(DisplayName="None"),
	Climb  = 1 UMETA(DisplayName="Climb"),
	Mantle = 2 UMETA(DisplayName="Mantle"),
	Vault  = 3 UMETA(DisplayName="Vault"),
	Max        UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ERootYawOffsetMode : uint8
{
	Accumulate = 0 UMETA(DisplayName="Accumulate"),
	BlendOut   = 1 UMETA(DisplayName="BlendOut"),
	Hold       = 2 UMETA(DisplayName="Hold"),
	Max            UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ELocomotionDirection : uint8
{
	Forward  = 0 UMETA(DisplayName="Forward"),
	Backward = 1 UMETA(DisplayName="Backward"),
	Right    = 2 UMETA(DisplayName="Right"),
	Left     = 3 UMETA(DisplayName="Left"),
	Max          UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEquipWeapon : uint8
{
	UnArmed = 0 UMETA(DisplayName="UnArmed"),
	Pistol  = 1 UMETA(DisplayName="Pistol"),
	Rifle   = 2 UMETA(DisplayName="Rifle"),
	Hammer  = 3 UMETA(DisplayName="Hammer"),
	Max         UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMoveType : uint8
{
	Walking   = 0 UMETA(DisplayName="Walking"),
	Jogging   = 1 UMETA(DisplayName="Jogging"),
	Crouching = 2 UMETA(DisplayName="Crouching"),
	Max           UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FWeaponSocketNames : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName PistolUnEquipe      = FName("PistolUnEquipe");
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName RifleUnEquipe       = FName("RifleUnEquipe");
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName WeaponEquiped       = FName("WeaponEquiped");
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName HammerWeaponEquip   = FName("HammerEquip");
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName HammerWeaponUnequip = FName("HammerUnequip");
};

USTRUCT(BlueprintType)
struct FWeaponMovementSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MaxWalkSpeed          = 250.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MaxAcceleration       = 250.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float BrakingDeceleration   = 250.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float BrakingFrictionFactor = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float BrakingFriction       = 0.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool  UseSeperateBrakingFriction = true;
	
	static bool GetMovementSetting(const UDataTable* InDataTable, const EEquipWeapon InWeaponType, const EMoveType InMoveType, FWeaponMovementSettings& OutSettings)
	{
		const FString WeaponStr = EnumUtil::GetEnumNameAsString(InWeaponType);
		const FString MoveStr   = EnumUtil::GetEnumNameAsString(InMoveType);

		const FString RowString = FString::Printf(TEXT("%s_%s"), *WeaponStr, *MoveStr);
		const FName RowName(*RowString);

		if (const FWeaponMovementSettings* FoundRow = InDataTable->FindRow<FWeaponMovementSettings>(RowName, TEXT("GetMovementSetting")))
		{
			OutSettings = *FoundRow;
			return true;
		}

		return false;
	}
};

namespace CollisionChannelNames
{
	// static FName Vehicle("Vehicle");
}

namespace CollisionChannels
{
	// static ECollisionChannel Vehicle = ECollisionChannel::ECC_GameTraceChannel1;
}

namespace ObjectTypeQuerys
{
	static EObjectTypeQuery Parkour = ObjectTypeQuery7;	
}

namespace PhysicalSurfaces
{
	static constexpr EPhysicalSurface Glass     = SurfaceType1;
	static constexpr EPhysicalSurface Concrete  = SurfaceType2;
	static constexpr EPhysicalSurface Character = SurfaceType3;
}
