// bong9 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "LeTabListWidget.generated.h"

class ULeCommonButtonBase;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class LEGACY_API ULeTabListWidget : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

public:
	void RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName);

private:
	//~ Begin UWidget Interface
#if WITH_EDITOR	
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Options Tab List Settings", meta=(AllowPrivateAccess=true, ClampMin="1", ClampMax="10"))
	int32 DebugEditorPreviewTabCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Options Tab List Settings", meta=(AllowPrivateAccess=true))
	TSubclassOf<ULeCommonButtonBase> TabButtonEntryWidgetClass;
};
