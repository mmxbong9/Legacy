// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Legacy/Character/LeCharacter.h"

#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealth_DeathEvent, AActor*, OwningActor, AActor*, Instigator);

UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class LEGACY_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();
	
	UPROPERTY(BlueprintAssignable)
	FHealth_DeathEvent OnDeathStarted;

	UPROPERTY(BlueprintAssignable)
	FHealth_DeathEvent OnDeathFinished;

public:
	virtual void InitializeComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void UpdateHealthUI() const;
	void UpdateShieldUI() const;
	
	void IncreaseHealth(float InAmount);
	void DecreaseHealth(float InAmount);
	void IncreaseShield(float InAmount);

private:
	UPROPERTY()
	TObjectPtr<ALeCharacter> LeCharacter;

	UFUNCTION()
	void OnRep_Health();
	
	UPROPERTY(ReplicatedUsing=OnRep_Health)
	float Health;
	float MaxHealth;

	UFUNCTION()
	void OnRep_Shield();
	
	UPROPERTY(ReplicatedUsing=OnRep_Shield)
	float Shield;
	float MaxShield;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="01.Settings|Death")
	TArray<TObjectPtr<UAnimMontage>> DeathMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="01.Settings|HitReact")
	TArray<TObjectPtr<UAnimMontage>> HitReacts;
};

