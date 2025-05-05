// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Legacy/Interfaces/AnimInterface.h"
#include "Legacy/Types/LeTypes.h"
#include "LeAnimInstance.generated.h"

UCLASS()
class LEGACY_API ULeAnimInstance : public UAnimInstance, public IAnimInterface
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float InDeltaSeconds) override;

private:
	void GetVelocityData();
	void GetAccelerationData();
	void GetLocationData();
	void GetRotationData    (const float InDeltaTime);
	void UpdateOrientationData();
	void GetCharacterStates (const float InDeltaTime);
	void UpdateRootYawOffset(const float InDeltaTime);
	void SetRootYawOffset(const float InAngle);
	void ProcessTurnYawCurve();

private:
	ELocomotionDirection CalculateLocomotionDirection(const float InCurrentLocomotionAngle, const ELocomotionDirection InCurrentDirection,
		const float InBackwardMin=-130.0f, const float InBackwardMax=130.0f, const float InForwardMin=-50,const float InForwardMax=50, const float InDeadZone=20);
	
	FVector CharacterVelocity;
	FVector CharacterVelocity2D;
	FVector Acceleration;
	FVector Acceleration2D;
	bool    bIsAcceleration;

	FVector LastFrameWorldLocation;
	FVector WorldLocation;
	float   DeltaLocation;

	FRotator WorldRotation;
	float    LastFrameActorYaw;
	float    ActorYaw;
	float    DeltaActorYaw;
	float    LeanAngle;
	float    AimPitch;

	ELocomotionDirection LastFrameVelocityLocomotionDirection;
	ELocomotionDirection VelocityLocomotionDirection;
	float VelocityLocomotionAngle;
	float VelocityLocomotionAngleWithOffset;
	
	ELocomotionDirection AccelerationLocomotionDirection;
	float AccelerationLocomotionAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess=true))
	EMoveType IncomingMoveType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Character", meta=(AllowPrivateAccess=true))
	EMoveType LastFrameMoveType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Character", meta=(AllowPrivateAccess=true))
	EMoveType CurrentMoveType;
	bool bIsMoveTypeChanged;

	bool bLastFrameIsCrouching;
	bool bIsCrouching;
	bool bCrouchStateChanged;
	
	bool bIsOnAir;
	bool bIsJumping;
	bool bIsFalling;

	float TimeToJumpApex;
	float TimeFalling;

	FFloatSpringState  RootYawOffsetSpringState;
	ERootYawOffsetMode RootYawOffsetMode;
    float RootYawOffset;

	float LastFrameTurnYawCurveValue;
	float TurnYawCurveValue;
	
public:
	//- AnimInstance Interface
	virtual void  ReceiveEquipWeapon(const EEquipWeapon InWeaponType) override;
	virtual void  ReceiveCurrentMoveType(const EMoveType InMoveType) override;
	virtual void  ReceiveGroundDistance(const float InGroundDistance) override;
	virtual void  ReceiveReplicatedAcceleration(const FVector InReplicatedAcceleration) override;
	virtual float GetVelocity() override;
	//- AnimInstance Interface

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess=true))
	EEquipWeapon EquipWeapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess=true))
	EMoveType MoveType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess=true))
	float GroundDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess=true))
	FVector ReplicatedAcceleration;
};
