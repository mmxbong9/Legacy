// Fill out your copyright notice in the Description page of Project Settings.

#include "VehicleInteractionComponent.h"

UVehicleInteractionComponent::UVehicleInteractionComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

	bVehicleInCharacter = false;
}

void UVehicleInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVehicleInteractionComponent::TickComponent(float InDeltaTime, ELevelTick InTickType, FActorComponentTickFunction* InThisTickFunction)
{
	Super::TickComponent(InDeltaTime, InTickType, InThisTickFunction);
}
