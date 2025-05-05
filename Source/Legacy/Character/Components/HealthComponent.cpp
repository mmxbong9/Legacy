// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Legacy/Legacy.h"
#include "Legacy/Interfaces/WidgetInterface.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	Health = 50.0f;
	MaxHealth = 150.0f;

	Shield = 10.0f;
	MaxShield = 99.0f;
}

void UHealthComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	LeCharacter = Cast<ALeCharacter>(GetOwner());
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, Health);
	DOREPLIFETIME(UHealthComponent, Shield);
}

void UHealthComponent::UpdateHealthUI() const
{
	const float HealthPercentage = UKismetMathLibrary::MapRangeClamped(Health, 0.0f, MaxHealth, 0.0f, 100.0f);
	LeCharacter->GetHealthBar()->SetScalarParameterValueOnMaterials(FName("HealthPercent"), HealthPercentage);
}

void UHealthComponent::UpdateShieldUI() const
{
	if (IWidgetInterface* Interface = Cast<IWidgetInterface>(LeCharacter->GetShieldWidget()->GetUserWidgetObject()))
	{
		Interface->UpdateShieldAmount(Shield);
	}
}

void UHealthComponent::IncreaseHealth(float InAmount)
{
	Health + InAmount > MaxHealth ? Health = MaxHealth : Health += InAmount;
	UpdateHealthUI();
}

void UHealthComponent::DecreaseHealth(float InAmount)
{
	float TempShieldAmount = 0.0f;

	/*
	 * 1. 쉴드만 차감
	 * 2. 쉴드와 체력이 함께 차감
	 * 3. 체력민 치김
	 * 4. 체력이 0이 되어 Death
	 */
	if (Shield >= InAmount)
	{
		Shield -= InAmount;
		UpdateShieldUI();
		
		FString Log = FString::Format(TEXT("Shield absorbed {0} | Remaining Shield {1}"), { InAmount,Shield });
		LOG_NET_FUNC(Log);
	}
	else
	{
		if (Shield <= 0.0f)
		{
			TempShieldAmount = InAmount;
		}
		else
		{
			TempShieldAmount = InAmount - Shield;

			FString Log = FString::Format(TEXT("Shield partially absorbed {0} | Remaining damage {1}"), { Shield,TempShieldAmount });
			LOG_NET_FUNC(Log);

			Shield = 0.0f;
			UpdateShieldUI();
		}

		if (TempShieldAmount >= Health)
		{
			LOG_NET_FUNC("[Damage] Lethal hit! Health reduced to 0.");
			Health = 0.0f;

			LOG_NET_FUNC("TODO >>> Play Dead Montage");
		}
		else
		{
			Health -= TempShieldAmount;
			
			FString Log = FString::Format(TEXT("[Damage] Health reduced by {0} | Remaining Health {1}"), { TempShieldAmount,Health });
			LOG_NET_FUNC(Log);
		}

		UpdateHealthUI();
	}
}

void UHealthComponent::IncreaseShield(float InAmount)
{
	Shield + InAmount > MaxShield ? Shield = MaxShield : Shield += InAmount;
	UpdateShieldUI();
}

void UHealthComponent::OnRep_Health()
{
	if (!LeCharacter || !LeCharacter->GetMesh() && !LeCharacter->GetMesh()->GetAnimInstance() || HitReacts.IsEmpty())
		return;
	
	UpdateHealthUI();

	LOG_NET_FUNC_FLOAT("Current Health", Health);

	const int32 MontageSelectionIndex = FMath::RandRange(0, HitReacts.Num() - 1);
	if (GetNetMode() != NM_DedicatedServer && !LeCharacter->IsLocallyControlled())
	{
		if ( 0.0f < LeCharacter->GetMesh()->GetAnimInstance()->Montage_Play(HitReacts[MontageSelectionIndex]) )
		{
			LOG_NET_FUNC("Start HitReaction Montage");
		}
	}
}

void UHealthComponent::OnRep_Shield()
{
	UpdateShieldUI();
	
	LOG_NET_FUNC_FLOAT("Current Shield", Shield);
}