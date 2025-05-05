// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameDropSubsystem.h"

UFrameDropSubsystem* UFrameDropSubsystem::Get(const UWorld* InWorld)
{
	if (!InWorld) return nullptr;
	return InWorld->GetSubsystem<UFrameDropSubsystem>();
}

TStatId UFrameDropSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFrameDropSubsystem, STATGROUP_Tickables);
}

void UFrameDropSubsystem::Tick(const float InDeltaTime)
{
	bIsFrameDropping = InDeltaTime > FrameDropThreshold;

	// if (bIsFrameDropping)
	// {
	// 	float FPS = (InDeltaTime > 0.f) ? (1.0f / InDeltaTime) : 0.f;
	// 	UE_LOG(LogTemp, Warning, TEXT("Find FrameDropping FPS: %.3f ( DeltaTime:%.3f )"), FPS, InDeltaTime);
	// }
}
