// Fill out your copyright notice in the Description page of Project Settings.

#include "AsyncSpawnSubsystem.h"
#include "FrameDropSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

UAsyncSpawnSubsystem* UAsyncSpawnSubsystem::Get(const UWorld* InWorld)
{
	if (!InWorld) return nullptr;
	return InWorld->GetSubsystem<UAsyncSpawnSubsystem>();
}

void UAsyncSpawnSubsystem::Tick(float InDeltaTime)
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	if (!World->GetSubsystem<UFrameDropSubsystem>()->IsFrameDropping() && !SpawnQueue.IsEmpty())
	{
		SpawnQueue.Sort([](const FSpawnRequest& A, const FSpawnRequest& B)
		{
			return A.PriorityDistanceSqr < B.PriorityDistanceSqr;
		});
		
		FSpawnRequest Request = SpawnQueue[0];
		if (Request.ActorClass.IsValid())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AActor* DeferredActor = World->SpawnActorDeferred<AActor>(Request.ActorClass.Get(), Request.Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			if (DeferredActor)
			{
				DeferredActor->FinishSpawning(Request.Transform);
				SpawnQueue.RemoveAt(0);
				
				UE_LOG(LogTemp, Warning, TEXT("AsyncSpawnSubsystem: PriorityDistanceSqr:%2.f SpawnedLocation: %s "),
					Request.PriorityDistanceSqr, *Request.Transform.GetLocation().ToString())
			}
			else
			{
				Request.RetryCount++;
				if (Request.RetryCount >= MaxRetryCount)
				{
					UE_LOG(LogTemp, Error, TEXT("AsyncSpawnSubsystem: Failed to spawn actor %s at %s after %d attempts."),
						*Request.ActorClass.ToString(), *Request.Transform.GetLocation().ToString(), Request.RetryCount);
					SpawnQueue.RemoveAt(0);
				}
			}
		}
	}
}

void UAsyncSpawnSubsystem::QueueAsyncSpawn(TSoftClassPtr<AActor> InActorClass, const FTransform& InTransform)
{
	const float DistSqr = GetDistanceSqrToLocalPlayer(InTransform.GetLocation());
		
	if (InActorClass.IsValid())
	{
		SpawnQueue.Add(FSpawnRequest(InActorClass, InTransform, DistSqr));
	}
	else
	{
		StreamableManager.RequestAsyncLoad(InActorClass.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda([this, InActorClass, InTransform, DistSqr]()
			{
				SpawnQueue.Add(FSpawnRequest(InActorClass, InTransform, DistSqr));
			})
		);
	}
}

float UAsyncSpawnSubsystem::GetDistanceSqrToLocalPlayer(const FVector& Location) const
{
	if (const UWorld* World = GetWorld())
	{
		if (const APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
		{
			if (const APawn* Pawn = PC->GetPawn())
			{
				return FVector::DistSquared(Location, Pawn->GetActorLocation());
			}
		}
	}
	return MAX_flt;
}
