// bong9 All Rights Reserved

#include "LeTabListWidget.h"
#include "LeCommonButtonBase.h"
#include "Editor/WidgetCompilerLog.h"

void ULeTabListWidget::RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName)
{
	RegisterTab(InTabID, TabButtonEntryWidgetClass, nullptr);

	if (ULeCommonButtonBase* FoundButton = Cast<ULeCommonButtonBase>(GetTabButtonBaseByID(InTabID)))
	{
		FoundButton->SetButtonText(InTabDisplayName);
	}
}

#if WITH_EDITOR
void ULeTabListWidget::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(
			TEXT("The variable TabButtonEntryWidgetClass has no valid entry specified. ") +
			GetClass()->GetName() +
			TEXT(" need a valid entry widget class to function properly")
		));
	}
}
#endif
