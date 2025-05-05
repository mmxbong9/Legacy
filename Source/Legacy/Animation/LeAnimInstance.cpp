// Fill out your copyright notice in the Description page of Project Settings.

#include "LeAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void ULeAnimInstance::NativeUpdateAnimation(float InDeltaSeconds)
{
	Super::NativeUpdateAnimation(InDeltaSeconds);

	// if (!IsValid(TryGetPawnOwner())) return;
	//
	// GetVelocityData();
	// GetAccelerationData();
	// GetLocationData();
	// UpdateOrientationData();
	// GetRotationData    (InDeltaSeconds);
	// GetCharacterStates (InDeltaSeconds);
	// UpdateRootYawOffset(InDeltaSeconds);
}

void ULeAnimInstance::GetVelocityData()
{
	CharacterVelocity   = TryGetPawnOwner()->GetMovementComponent()->Velocity;
	CharacterVelocity2D = CharacterVelocity * FVector(1, 1, 0);
}

void ULeAnimInstance::GetAccelerationData()
{
	if ( const UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(TryGetPawnOwner()->GetMovementComponent() ))
	{
		Acceleration    = MovementComponent->GetCurrentAcceleration();
		Acceleration2D  = Acceleration * FVector(1, 1, 0);
		bIsAcceleration = !UKismetMathLibrary::NearlyEqual_FloatFloat(UKismetMathLibrary::VSizeXY(Acceleration2D), 0.0f);
	}
}

void ULeAnimInstance::GetLocationData()
{
	LastFrameWorldLocation = WorldLocation;
	WorldLocation = TryGetPawnOwner()->GetActorLocation();
	DeltaLocation = UKismetMathLibrary::VSize(WorldLocation - LastFrameWorldLocation);
}

void ULeAnimInstance::GetRotationData(const float InDeltaTime)
{
	WorldRotation = TryGetPawnOwner()->GetActorRotation();
	LastFrameActorYaw = ActorYaw;
	ActorYaw = WorldRotation.Yaw;

	float VelocityLocomotionDirectionSelectValue = 0.0f;
	switch (VelocityLocomotionDirection)
	{
	case ELocomotionDirection::Forward:  VelocityLocomotionDirectionSelectValue =  1.0f; break;
	case ELocomotionDirection::Backward: VelocityLocomotionDirectionSelectValue = -1.0f; break;
	case ELocomotionDirection::Right:    VelocityLocomotionDirectionSelectValue =  0.0f; break;
	case ELocomotionDirection::Left:     VelocityLocomotionDirectionSelectValue =  0.0f; break;
	default: break;
	}
	
	DeltaActorYaw = (ActorYaw - LastFrameActorYaw);
	LeanAngle = UKismetMathLibrary::ClampAngle(DeltaActorYaw / InDeltaTime / 6.0f * VelocityLocomotionDirectionSelectValue, -90.0f, 90.0);
	AimPitch = UKismetMathLibrary::NormalizeAxis(TryGetPawnOwner()->GetBaseAimRotation().Pitch);
}

void ULeAnimInstance::UpdateOrientationData()
{
	LastFrameVelocityLocomotionDirection = VelocityLocomotionDirection;
	VelocityLocomotionAngle = UKismetAnimationLibrary::CalculateDirection(CharacterVelocity2D, WorldRotation);
	VelocityLocomotionAngleWithOffset = UKismetMathLibrary::NormalizeAxis(VelocityLocomotionAngle - RootYawOffset);
	AccelerationLocomotionAngle = UKismetAnimationLibrary::CalculateDirection(Acceleration2D, WorldRotation);
	VelocityLocomotionDirection = CalculateLocomotionDirection(VelocityLocomotionAngle, VelocityLocomotionDirection);
	AccelerationLocomotionDirection = CalculateLocomotionDirection(AccelerationLocomotionAngle, AccelerationLocomotionDirection);;
}

void ULeAnimInstance::GetCharacterStates(const float InDeltaTime)
{
	LastFrameMoveType = CurrentMoveType;
	CurrentMoveType = IncomingMoveType;
	bIsMoveTypeChanged = CurrentMoveType != LastFrameMoveType;

	bLastFrameIsCrouching = IncomingMoveType == EMoveType::Crouching;
	bCrouchStateChanged = bIsCrouching != bLastFrameIsCrouching;

	if (const UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(TryGetPawnOwner()->GetMovementComponent()))
	{
		bIsOnAir = EMovementMode::MOVE_Falling == MovementComponent->GetGroundMovementMode() || EMovementMode::MOVE_Flying == MovementComponent->GetGroundMovementMode();
		bIsJumping = bIsOnAir && CharacterVelocity.Z > 0.0f;
		bIsFalling = bIsOnAir && CharacterVelocity.Z < 0.0f;
		if (bIsJumping)
		{
			TimeToJumpApex = (0.0f - CharacterVelocity.Z) / (MovementComponent->GetGravityZ());  
		}
		else
		{
			TimeToJumpApex = 0.0f;
		}
	}

	if (bIsFalling)
	{
		TimeFalling = TimeFalling + InDeltaTime;
	}
	else
	{
		if (bIsJumping)
		{
			TimeFalling = 0.0f;
		}
	}
}

void ULeAnimInstance::UpdateRootYawOffset(const float InDeltaTime)
{
	switch (RootYawOffsetMode)
	{
	case ERootYawOffsetMode::Accumulate:
		{
			SetRootYawOffset(RootYawOffset + (DeltaActorYaw * -1.0f));
		}
		break;
	case ERootYawOffsetMode::BlendOut:
		{
			const float Angle = UKismetMathLibrary::FloatSpringInterp(RootYawOffset, 0.0f, RootYawOffsetSpringState,
				80.0f, 1.0f, InDeltaTime, 1.0f, 0.5f);
			
			SetRootYawOffset(Angle);
		}
		break;
	default: break;
	}

	RootYawOffsetMode = ERootYawOffsetMode::BlendOut;
}

void ULeAnimInstance::SetRootYawOffset(const float InAngle)
{
	RootYawOffset = UKismetMathLibrary::NormalizeAxis(InAngle);
}

void ULeAnimInstance::ProcessTurnYawCurve()
{
	LastFrameTurnYawCurveValue = TurnYawCurveValue;
	if (UKismetMathLibrary::NearlyEqual_FloatFloat(GetCurveValue(FName("IsTurning")), 0.0f))
	{
		TurnYawCurveValue = 0.0f;
		LastFrameTurnYawCurveValue = 0.0f;
	}
	else
	{
		TurnYawCurveValue = UKismetMathLibrary::SafeDivide(GetCurveValue(FName("root_rotation_Z")), GetCurveValue(FName("IsTurning")));
		if (LastFrameTurnYawCurveValue != 0.0f)
		{
			const float Offset = RootYawOffset - (TurnYawCurveValue-LastFrameTurnYawCurveValue);
			SetRootYawOffset(Offset);
		}
	}
}

ELocomotionDirection ULeAnimInstance::CalculateLocomotionDirection(const float InCurrentLocomotionAngle, const ELocomotionDirection InCurrentDirection,
                                                                   const float InBackwardMin, const float InBackwardMax, const float InForwardMin, const float InForwardMax, const float InDeadZone)
{
	switch (InCurrentDirection)
	{
		case ELocomotionDirection::Forward:
		{
			if (UKismetMathLibrary::InRange_FloatFloat(InCurrentLocomotionAngle, InDeadZone - InForwardMin, InDeadZone + InForwardMax))
				return ELocomotionDirection::Forward;
		}
		break;
	case ELocomotionDirection::Backward:
		{
			if (InCurrentLocomotionAngle < (InBackwardMin + InDeadZone) || InCurrentLocomotionAngle > (InBackwardMax - InDeadZone))
				return ELocomotionDirection::Backward;
		}
		break;
	case ELocomotionDirection::Right:
		{
			if (UKismetMathLibrary::InRange_FloatFloat(InCurrentLocomotionAngle, InForwardMax - InDeadZone, InBackwardMax + InDeadZone))
				return ELocomotionDirection::Right;
		}
		break;
	case ELocomotionDirection::Left:
		{
			if (UKismetMathLibrary::InRange_FloatFloat(InCurrentLocomotionAngle, InBackwardMin - InDeadZone, InForwardMin + InDeadZone))
				return ELocomotionDirection::Left;
		}
		break;
	default: break;
	}

	if (InCurrentLocomotionAngle < InBackwardMin || InCurrentLocomotionAngle > InBackwardMax)
	{
		return ELocomotionDirection::Backward;
	}
	else
	{
		if (UKismetMathLibrary::InRange_FloatFloat(InCurrentLocomotionAngle, InForwardMin, InForwardMin))
		{
			return ELocomotionDirection::Forward;
		}
		else
		{
			if (0.f < InCurrentLocomotionAngle )
			{
				return ELocomotionDirection::Right;
			}
			else
			{
				return ELocomotionDirection::Left;
			}
		}
	}
}

void ULeAnimInstance::ReceiveEquipWeapon(const EEquipWeapon InWeaponType)
{
	EquipWeapon = InWeaponType;
}

void ULeAnimInstance::ReceiveCurrentMoveType(const EMoveType InMoveType)
{
	IncomingMoveType = InMoveType;
}

void ULeAnimInstance::ReceiveGroundDistance(const float InGroundDistance)
{
	GroundDistance = InGroundDistance;
}

void ULeAnimInstance::ReceiveReplicatedAcceleration(const FVector InReplicatedAcceleration)
{
	ReplicatedAcceleration = InReplicatedAcceleration;
}

float ULeAnimInstance::GetVelocity()
{
	return 0.f;
}
