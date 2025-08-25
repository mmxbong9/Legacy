// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Legacy/AnimNotify/LeCollisionActivationData.h"
#include "LeCollision.generated.h"

struct FGameplayEffectSpecHandle;

class UBoxComponent;
class USphereComponent;

UCLASS()
class LEGACY_API ALeCollision : public AActor
{
	GENERATED_BODY()

public:
	ALeCollision();

	//~ Begin UObject Interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~ End UObject Interface

	void Initialize(const FCollisionActivationData& CollisionData);

protected:
	//~ Begin AActor Interface
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor Interface

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnRep_CollisionActivationData();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastShowDebugCollision();
	
	UPROPERTY(ReplicatedUsing=OnRep_CollisionActivationData)
	FCollisionActivationData CachedCollisionData;

private:
	void SelectCollisionComponent();

	void PrepEffectSpecs();

	void HandleMovement(AActor* OtherActor);

	void ApplyGameplayEffects(AActor* OtherActor);
	
	UPROPERTY(Replicated)
	TObjectPtr<USphereComponent> CollisionSphere;
	
	UPROPERTY(Replicated)
	TObjectPtr<UBoxComponent> CollisionBox;
	
	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultRootComponent;

	UPROPERTY(Transient)
	TObjectPtr<UPrimitiveComponent> ActiveCollisionComponent;
	
	UPROPERTY(Transient)
	TArray<AActor*> IgnoreActors;

	TArray<FGameplayEffectSpecHandle> CachedEffectSpecs;
};
