// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Legacy/Types/LeTypes.h"

#include "ParkourComponent.generated.h"

class ALeCharacter;
class UMotionWarpingComponent;

UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class LEGACY_API UParkourComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UParkourComponent();

	virtual void InitializeComponent() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void DoParkour();

public:
	//- Accessors
	EParkour GetParkourAction() const { return ParkourAction; }
	//- Accessors
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(AllowPrivateAccess=true))
	EParkour ParkourAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(AllowPrivateAccess=true))
	TEnumAsByte<EObjectTypeQuery> ObstacleDetectionChannel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta=(AllowPrivateAccess=true))
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta=(AllowPrivateAccess=true))
	TObjectPtr<ALeCharacter> Character;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="01.Settings|Parkour", Meta=(AllowPrivateAccess=true))
	TObjectPtr<UAnimMontage> ClimbMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="01.Settings|Parkour", Meta=(AllowPrivateAccess=true))
	TObjectPtr<UAnimMontage> MantleMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="01.Settings|Parkour", Meta=(AllowPrivateAccess=true))
	TObjectPtr<UAnimMontage> VaultMontage;
};
