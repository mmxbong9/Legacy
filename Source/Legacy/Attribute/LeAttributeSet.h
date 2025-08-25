// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "LeAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \

/**
 * 
 */
UCLASS()
class LEGACY_API ULeAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category=Attributes)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(ULeAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category=Attributes)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(ULeAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Energy, Category=Attributes)
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(ULeAttributeSet, Energy);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxEnergy, Category=Attributes)
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(ULeAttributeSet, MaxEnergy);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PhysicalAttack, Category=Attributes)
	FGameplayAttributeData PhysicalAttack;
	ATTRIBUTE_ACCESSORS(ULeAttributeSet, PhysicalAttack);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PhysicalResistance, Category=Attributes)
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(ULeAttributeSet, PhysicalResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Vitality, Category=Attributes)
	FGameplayAttributeData Vitality;
	ATTRIBUTE_ACCESSORS(ULeAttributeSet, Vitality);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Intelligence, Category=Attributes)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(ULeAttributeSet, Intelligence);
	
	UPROPERTY(BlueprintReadOnly, Category=Attributes)
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(ULeAttributeSet, IncomingDamage);

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Energy(const FGameplayAttributeData& OldEnergy) const;

	UFUNCTION()
	void OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy) const;

	UFUNCTION()
	void OnRep_PhysicalAttack(const FGameplayAttributeData& OldPhysicalAttack) const;

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;

	UFUNCTION()
	void OnRep_Vitality(const FGameplayAttributeData& OldVitality) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
};
