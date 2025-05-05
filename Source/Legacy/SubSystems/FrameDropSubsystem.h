// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FrameDropSubsystem.generated.h"

UCLASS()
class LEGACY_API UFrameDropSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	static UFrameDropSubsystem* Get(const UWorld* InWorld);
	
	FORCEINLINE bool IsFrameDropping() const { return bIsFrameDropping; }

	virtual TStatId GetStatId()          const override;
	virtual bool    IsTickable()         const override { return true; }

	virtual void Tick(const float InDeltaTime) override;

private:
	bool bIsFrameDropping = false;

	// 12.5fps(0.08f), 30fps(0.033f) 60fps(0.016f) 
	float FrameDropThreshold = 0.08f;

	static UFrameDropSubsystem* Instance;
};
