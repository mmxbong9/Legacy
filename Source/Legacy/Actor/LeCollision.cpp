// bong9 All Rights Reserved

#include "LeCollision.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Legacy/Ability/LeGameplayAbility.h"
#include "Legacy/GameplayTags/LeGameplayTags.h"
#include "Legacy/Global/GlobalUtil.h"
#include "Legacy/Interfaces/HitInterface.h"
#include "Net/UnrealNetwork.h"

ALeCollision::ALeCollision()
{
	PrimaryActorTick.bCanEverTick = false;;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(GET_MEMBER_NAME(DefaultRootComponent));
	RootComponent = DefaultRootComponent;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(GET_MEMBER_NAME(CollisionSphere));
	CollisionSphere->SetupAttachment(GetRootComponent());

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(GET_MEMBER_NAME(CollisionBox));
	CollisionBox->SetupAttachment(GetRootComponent());

	bReplicates = true;
	
	AActor::SetReplicateMovement(true);
}

void ALeCollision::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALeCollision, CollisionSphere);
	DOREPLIFETIME(ALeCollision, CollisionBox);
}

void ALeCollision::Initialize(const FCollisionActivationData& CollisionData)
{
	if(!HasAuthority()) return;

	IgnoreActors.Empty();
	CachedCollisionData = CollisionData;

	SelectCollisionComponent();

	if (!ActiveCollisionComponent) return;

	ActiveCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ActiveCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ALeCollision::OnOverlapBegin);

	if (CollisionData.bShowDebugCollision)
	{
		MulticastShowDebugCollision();
	}

	PrepEffectSpecs();
}

void ALeCollision::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
}

void ALeCollision::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner() || OtherActor->GetOwner() == GetOwner() || IgnoreActors.Contains(OtherActor)) return;

	IgnoreActors.Add(OtherActor);
	
	HandleMovement(OtherActor);
	ApplyGameplayEffects(OtherActor);
}

void ALeCollision::OnRep_CollisionActivationData()
{
	SelectCollisionComponent();
}

void ALeCollision::MulticastShowDebugCollision_Implementation()
{
	if (GetOwner() && !GetOwner()->HasLocalNetOwner()) return;

	if (ActiveCollisionComponent)
	{
		ActiveCollisionComponent->SetHiddenInGame(false);
	}
}

void ALeCollision::SelectCollisionComponent()
{
	switch (CachedCollisionData.ShapeSettings.CollisionShapeType)
	{
	case ECollisionShapeTypes::Sphere:
		ActiveCollisionComponent = CollisionSphere;
		CollisionSphere->SetSphereRadius(CachedCollisionData.ShapeSettings.SphereRadius);
		CollisionSphere->SetLineThickness(5.f);
		break;
	case ECollisionShapeTypes::Box:
		ActiveCollisionComponent = CollisionBox;
		CollisionBox->SetBoxExtent(CachedCollisionData.ShapeSettings.BoxExtent);
		CollisionBox->SetRelativeLocation(CachedCollisionData.ShapeSettings.RelativeOffset);
		CollisionBox->SetLineThickness(5.f);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Invalid Collision Shape Type"));
		break;
	}
}

void ALeCollision::PrepEffectSpecs()
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (!ASC) return;

	ULeGameplayAbility* Ability = Cast<ULeGameplayAbility>(ASC->GetAnimatingAbility());
	if (!Ability) return;

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(GetOwner());
	EffectContext.AddInstigator(GetOwner(), this);

	for (const TSubclassOf<UGameplayEffect>& EffectClass : CachedCollisionData.EffectClasses)
	{
		if (IsValid(EffectClass))
		{
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, Ability->GetAbilityLevel(), EffectContext);
			CachedEffectSpecs.Add(SpecHandle);
		}
	}

	if (CachedCollisionData.DamageEffectClass)
	{
		float Damage = CachedCollisionData.Damage;
		
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CachedCollisionData.DamageEffectClass, Ability->GetAbilityLevel(), EffectContext);

		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FLeGameplayTags::Get().Damage, Damage);
		
		CachedEffectSpecs.Add(SpecHandle);
	}
}

void ALeCollision::HandleMovement(AActor* OtherActor)
{
	if (!OtherActor) return;

	const auto& MoveSettings = CachedCollisionData.MovementSettings;

	if (MoveSettings.bShouldMoveActor)
	{
		AActor* ActorToMove = nullptr;
		if (MoveSettings.ActorToMove == EMoveTarget::Owner)
		{
			ActorToMove = GetOwner();
		}
		if (MoveSettings.ActorToMove == EMoveTarget::HitActor)
		{
			ActorToMove = OtherActor;
		}

		if (ActorToMove && ActorToMove->Implements<UHitInterface>())
		{
			FVector InstigatorForward = GetOwner()->GetActorForwardVector();
			InstigatorForward.Z = 0.f;
			InstigatorForward.Normalize();

			FVector PushDirection = InstigatorForward;
			if (MoveSettings.MoveDirection == EMoveDirection::Forward)
			{
				PushDirection *= -1;
			}

			IHitInterface::Execute_PushActor(ActorToMove, PushDirection, MoveSettings.MoveDistance);
		}
	}
}

void ALeCollision::ApplyGameplayEffects(AActor* OtherActor)
{
	if (!OtherActor) return;

	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		for (const FGameplayEffectSpecHandle& EffectSpecHandle : CachedEffectSpecs)
		{
			if (EffectSpecHandle.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
			}
		}
	}
}
