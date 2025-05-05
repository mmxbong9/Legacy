// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/StreamableManager.h"
#include "UObject/SoftObjectPtr.h"
#include "AsyncSpawnSubsystem.generated.h"

class UFrameDropSubsystem;

USTRUCT()
struct FSpawnRequest
{
	GENERATED_BODY()

	TSoftClassPtr<AActor> ActorClass;
	FTransform Transform;
	float PriorityDistanceSqr = MAX_flt;
	int32 RetryCount;
	
	FSpawnRequest() = default;
	FSpawnRequest(TSoftClassPtr<AActor> InClass, const FTransform& InTransform, float InPriority)
		: ActorClass(InClass), Transform(InTransform), PriorityDistanceSqr(InPriority), RetryCount(0) {}
};

UCLASS()
class LEGACY_API UAsyncSpawnSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	static UAsyncSpawnSubsystem* Get(const UWorld* InWorld);
	
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UAsyncSpawnSubsystem, STATGROUP_Tickables); }
	
	virtual void Tick(float InDeltaTime) override;
	virtual bool IsTickable()         const override { return true; }

	void QueueAsyncSpawn(TSoftClassPtr<AActor> InActorClass, const FTransform& InTransform);
	
private:
	float GetDistanceSqrToLocalPlayer(const FVector& Location) const;
	
	static UAsyncSpawnSubsystem* Instance;
	
	FStreamableManager StreamableManager;
	TArray<FSpawnRequest> SpawnQueue;
	
	constexpr static int32 MaxRetryCount = 10;
};
