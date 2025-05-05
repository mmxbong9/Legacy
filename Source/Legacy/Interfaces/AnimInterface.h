// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AnimInterface.generated.h"

enum class EMoveType : uint8;
enum class EEquipWeapon : uint8;

UINTERFACE(MinimalAPI)
class UAnimInterface : public UInterface
{
	GENERATED_BODY()
};

class LEGACY_API IAnimInterface
{
	GENERATED_BODY()

public:
	virtual void  ReceiveEquipWeapon(const EEquipWeapon InWeaponType) = 0;
	virtual void  ReceiveCurrentMoveType(const EMoveType InMoveType) = 0;
	virtual void  ReceiveGroundDistance(const float IncomingGroundDistance) = 0;
	virtual void  ReceiveReplicatedAcceleration(const FVector InReplicatedAcceleration) = 0;
	virtual float GetVelocity() = 0;
};
