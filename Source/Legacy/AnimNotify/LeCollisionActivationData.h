#pragma once

#include "CoreMinimal.h"
#include "Legacy/Global/GlobalUtil.h"

#include "LeCollisionActivationData.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class ECollisionSocket : uint8
{
	RightWeapon    = 0 UMETA(DisplayName="Weapon_R"),
	LeftWeapon     = 1 UMETA(DisplayName="Weapon_L"),
	RightFoot      = 2 UMETA(DisplayName="Foot_R"),
	LeftFoot       = 3 UMETA(DisplayName="Foot_L"),
	RightHand      = 4 UMETA(DisplayName="Hand_R"),
	LeftHand       = 5 UMETA(DisplayName="Hand_L"),
	RightShoulder  = 6 UMETA(DisplayName="Shoulder_R"),
	LeftShoulder   = 7 UMETA(DisplayName="Shoulder_L"),
	Custom         = 8 UMETA(DisplayName="Custom"),
	Max            = 9 UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FCollisionSocketSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Collision)
	ECollisionSocket CollisionSocket = ECollisionSocket::Custom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Collision, meta=(EditCondition="CollisionSocket == ECollisionSocket::Custom"))
	FName CustomSocketName = NAME_None;
};

UENUM(BlueprintType)
enum class ECollisionShapeTypes : uint8
{
	Sphere = 0 UMETA(DisplayName="Sphere"),
	Box    = 1 UMETA(DisplayName="Box"),
	Max    = 2 UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FCollisionShapeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Collision)
	ECollisionShapeTypes CollisionShapeType = ECollisionShapeTypes::Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Collision, meta=(EditCondition="CollisionShapeType == ECollisionShapeTypes::Box", EditConditionHides))
	FVector BoxExtent = FVector(20.f, 20.f, 20.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Collision, meta=(EditCondition="CollisionShapeType == ECollisionShapeTypes::Sphere", EditConditionHides))
	float SphereRadius = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Collision)
	FVector RelativeOffset = FVector::ZeroVector;
};

UENUM(BlueprintType)
enum class EMoveTarget : uint8
{
	Owner    = 0 UMETA(DisplayName="Owner"),
	HitActor = 1 UMETA(DisplayName="HitActor"),
	Max      = 2 UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMoveDirection : uint8
{
	Forward  = 0 UMETA(DisplayName="Forward"),
	Backward = 1 UMETA(DisplayName="Backward"),
	Max      = 2 UMETA(Hidden)
};

USTRUCT(Blueprintable)
struct FMovementSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
	bool bShouldMoveActor = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement, meta=(EditCondition="bShouldMoveActor", EditConditionHides))
	EMoveTarget ActorToMove = EMoveTarget::HitActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement, meta=(EditCondition="bShouldMoveActor", EditConditionHides))
	EMoveDirection MoveDirection = EMoveDirection::Backward;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement, meta=(EditCondition="bShouldMoveActor", EditConditionHides))
	float MoveDistance = 25.f;
};

// --------------------------------------------------------------------------------------------------------

USTRUCT(Blueprintable)
struct FCollisionActivationData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Collision)
	FCollisionSocketSettings SocketSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Collision)
	FCollisionShapeSettings ShapeSettings;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Collision)
	TArray<TSubclassOf<UGameplayEffect>> EffectClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Collision)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Damage)
	float Damage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Collision)
	FMovementSettings MovementSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Debug)
	bool bShowDebugCollision = false;

	// ---------------------------------------------------------------------------------------------------//

	static FName GetSocketName(const FCollisionActivationData& Data)
	{
		if (Data.SocketSettings.CollisionSocket == ECollisionSocket::Custom)
		{
			if (Data.SocketSettings.CustomSocketName.IsNone())
			{
				UE_LOG(LogTemp, Error, TEXT("CollisionActivationData: Custom Socket chosen but CustomSocketName is None"));
			}
			return Data.SocketSettings.CustomSocketName;
		}

		return EnumUtil::GetEnumDisplayNameAsFName(Data.SocketSettings.CollisionSocket);
	}
};