// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define LOG_FUNC() UE_LOG(LogTemp, Warning, TEXT("[%s]"), *FString(__FUNCTION__));
#define LOG_WARN(fmt, ...) UE_LOG(LogTemp, Warning, TEXT("[%s] " fmt), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) UE_LOG(LogTemp, Error, TEXT("[%s] " fmt), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) UE_LOG(LogTemp, Display, TEXT("[%s] " fmt), *FString(__FUNCTION__), ##__VA_ARGS__)

#define LOG_NET_FUNC(Message) \
{ \
AActor* LogOwner = nullptr; \
if (const UObject* ThisObject = Cast<UObject>(this)) \
{ \
if (const AActor* AsActor = Cast<AActor>(ThisObject)) \
{ \
LogOwner = const_cast<AActor*>(AsActor); \
} \
else if (const UActorComponent* AsComponent = Cast<UActorComponent>(ThisObject)) \
{ \
LogOwner = AsComponent->GetOwner(); \
} \
} \
if (IsValid(LogOwner)) \
{ \
const FString RoleString = UEnum::GetValueAsString(LogOwner->GetLocalRole()); \
UE_LOG(LogTemp, Log, TEXT("[%s][%s] %s"), *RoleString, TEXT(__FUNCTION__), *FString(Message)); \
} \
else \
{ \
UE_LOG(LogTemp, Warning, TEXT("[UnknownRole][%s] %s"), TEXT(__FUNCTION__), *FString(Message)); \
} \
}

#define LOG_NET_FUNC_FLOAT(Message, FloatValue) \
{ \
AActor* LogOwner = nullptr; \
if (const UObject* ThisObject = Cast<UObject>(this)) \
{ \
if (const AActor* AsActor = Cast<AActor>(ThisObject)) \
{ \
LogOwner = const_cast<AActor*>(AsActor); \
} \
else if (const UActorComponent* AsComponent = Cast<UActorComponent>(ThisObject)) \
{ \
LogOwner = AsComponent->GetOwner(); \
} \
} \
if (IsValid(LogOwner)) \
{ \
const FString RoleString = UEnum::GetValueAsString(LogOwner->GetLocalRole()); \
UE_LOG(LogTemp, Log, TEXT("[%s][%s] %s: %.2f"), *RoleString, TEXT(__FUNCTION__), *FString(Message), FloatValue); \
} \
else \
{ \
UE_LOG(LogTemp, Warning, TEXT("[UnknownRole][%s] %s: %.2f"), TEXT(__FUNCTION__), *FString(Message), FloatValue); \
} \
}