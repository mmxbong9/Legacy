// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "LeCollisionActivationData.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "LeCollisionNotifyState.generated.h"

class ALeCollision;
/**
 * 
 */
UCLASS()
class LEGACY_API ULeCollisionNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision Settings")
	FCollisionActivationData CollisionData;

private:
	TWeakObjectPtr<ALeCollision> Collision;
};
