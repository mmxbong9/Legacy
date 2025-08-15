#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "Components/WidgetComponent.h"
#include "CharacterHelper.generated.h"

#define LOAD_ASSET_ADD_TO_ARRAY(Array, ObjectType, Path) { \
	static ConstructorHelpers::FObjectFinder<ObjectType> Asset(TEXT(Path)); \
		if (Asset.Succeeded()) { Array.Add(Asset.Object); } \
}

#define LOAD_OBJECT_ASSIGN(ObjectVar, ObjectType, Path) { \
	static ConstructorHelpers::FObjectFinder<ObjectType> Asset(TEXT(Path)); \
		if (Asset.Succeeded()) { ObjectVar = Asset.Object; } \
}

#define LOAD_CLASS_ASSIGN(ClassVar, ClassType, Path) { \
	static ConstructorHelpers::FClassFinder<ClassType> Asset(TEXT(Path)); \
		if (Asset.Class != nullptr) { ClassVar = Asset.Class; } \
}

#define LOAD_ASSET_SET_SKELETAL(ObjectVar, Path) { \
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset(TEXT(Path)); \
		if (Asset.Succeeded()) { ObjectVar->SetSkeletalMesh(Asset.Object); } \
}


UCLASS()
class LEGACY_API UCharacterHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UEnhancedInputLocalPlayerSubsystem* GetInputLocalPlayerSubsystem(const TObjectPtr<AController>& InController);

	static UStaticMeshComponent* CreateStaticMeshWithAsset(AActor* Owner, const TCHAR* Name, USceneComponent* Parent, const TCHAR* Socket, const TCHAR* Path)
	{
		UStaticMeshComponent* Comp = Owner->CreateDefaultSubobject<UStaticMeshComponent>(FName(Name));
		Comp->SetupAttachment(Parent, FName(Socket));

		ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(Path);
		if (Asset.Succeeded())
		{
			Comp->SetStaticMesh(Asset.Object);
		}
		return Comp;
	}

	static USkeletalMeshComponent* CreateSkeletalMeshWithAsset(AActor* Owner, const TCHAR* Name, USceneComponent* Parent, const TCHAR* Socket, const TCHAR* Path)
	{
		USkeletalMeshComponent* Comp = Owner->CreateDefaultSubobject<USkeletalMeshComponent>(FName(Name));
		Comp->SetupAttachment(Parent, FName(Socket));

		ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset(Path);
		if (Asset.Succeeded())
		{
			Comp->SetSkeletalMesh(Asset.Object);
		}
		return Comp;
	}

	static UWidgetComponent* CreateWidgetCompWithClass(AActor* Owner, const TCHAR* Name, USceneComponent* Parent, const TCHAR* Socket, const TCHAR* WidgetBPPath, EWidgetSpace Space, FVector2D DrawSize)
	{
		UWidgetComponent* WidgetComp = Owner->CreateDefaultSubobject<UWidgetComponent>(FName(Name));
		WidgetComp->SetupAttachment(Parent, FName(Socket));
		WidgetComp->SetWidgetSpace(Space);
		WidgetComp->SetDrawSize(DrawSize);

		static ConstructorHelpers::FClassFinder<UUserWidget> Finder(WidgetBPPath);
		if (Finder.Succeeded()) { WidgetComp->SetWidgetClass(Finder.Class); }
		return WidgetComp;
	}
};
