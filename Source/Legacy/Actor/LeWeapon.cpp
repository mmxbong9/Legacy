// bong9 All Rights Reserved

#include "LeWeapon.h"

#include "Legacy/Character/LeCharacter.h"
#include "Legacy/Global/GlobalUtil.h"

ALeWeapon::ALeWeapon()
{
	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(GET_MEMBER_NAME(WeaponSkeletalMesh));
	WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponSkeletalMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECC_Visibility,   ECR_Block);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECC_WorldStatic,  ECR_Block);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECC_Pawn,         ECR_Overlap);
	WeaponSkeletalMesh->SetupAttachment(GetRootComponent());
	WeaponSkeletalMesh->SetHiddenInGame(true);
	WeaponSkeletalMesh->SetSkeletalMesh(ConstructorUtil::FindAsset<USkeletalMesh>(TEXT("/Game/ALS/SkeletalMeshs/Hammer/SKM_Hammer")));

	SetRootComponent(WeaponSkeletalMesh.Get());
}

void ALeWeapon::AttachToCharacter(const FName& AttachmentPoint)
{
	if (AttachmentPoint == NAME_None) return;

	ALeCharacter* Character = Cast<ALeCharacter>(GetOwner());
	if (!Character) return;

	WeaponSkeletalMesh->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachmentPoint);
	WeaponSkeletalMesh->SetHiddenInGame(false);
}
