// bong9 All Rights Reserved

#include "LeCollisionNotifyState.h"

#include "Legacy/Actor/LeCollision.h"
#include "Legacy/Character/LeCharacter.h"
#include "Legacy/Character/Components/CombatComponent.h"

void ULeCollisionNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	if (ALeCharacter* OwnerCharacter = Cast<ALeCharacter>(MeshComp->GetOwner()))
	{
		if (OwnerCharacter->HasAuthority())
		{
			if (UCombatComponent* CombatComponent = OwnerCharacter->GetCombatComponent())
			{
				Collision = CombatComponent->SpawnAndActivateCollision(CollisionData);
			}
		}
	}
}

void ULeCollisionNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	if (ALeCharacter* OwnerCharacter = Cast<ALeCharacter>(MeshComp->GetOwner()))
	{
		if (OwnerCharacter->HasAuthority())
		{
			if (Collision.IsValid())
			{
				Collision->Destroy();
			}
		}
	}
}
