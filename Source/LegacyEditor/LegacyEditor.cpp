// Fill out your copyright notice in the Description page of Project Settings.

#include "LegacyEditor.h"
#include "Modules/ModuleManager.h"
#include "ToolMenus.h"
#include "EnumJsonMenu/EnumJsonGenerator.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

#define LOCTEXT_NAMESPACE "LegacyEditor"
class FLegacyEditorModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{
		if (UToolMenus::IsToolMenuUIEnabled())
		{
			UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FLegacyEditorModule::RegisterMenus));
		}
	}
	
	virtual void ShutdownModule() override
	{
		if (UToolMenus::IsToolMenuUIEnabled())
		{
			UToolMenus::UnRegisterStartupCallback(this);
		}
	}

private:
    void RegisterMenus()
    {
        if (UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools"))
        {
            FToolMenuSection& Section = Menu->AddSection("LegacyEditor.EnumGen", LOCTEXT("LegacyEnumGenSec", "Legacy Tools"));
            Section.AddMenuEntry(
                "Legacy_GenerateEnumsFromJson",
                LOCTEXT("Legacy_GenerateEnumsFromJson", "Generate Enums from JSON"),
                LOCTEXT("Legacy_GenerateEnumsFromJson_TT", "Scan '<Project>/Enums' for *.json and write GeneratedTypes.h"),
                FSlateIcon(),
                FUIAction(FExecuteAction::CreateRaw(this, &FLegacyEditorModule::OnGenerateEnumsClicked))
            );
        }
    }

    void OnGenerateEnumsClicked()
    {
        const FString ProjectDir  = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
        const FString ProjectName = FApp::GetProjectName();

        // JSON 탐색 루트: <Project>/Enums
        const FString EnumRootDir = FPaths::Combine(ProjectDir, TEXT("EnumJsonData"));

    	// 출력 파일: <Project>/Source/<ProjectName>/Global/GeneratedTypes.h
    	const FString OutHeader = FPaths::Combine(ProjectDir, TEXT("Source"), ProjectName, TEXT("Global"), TEXT("GeneratedTypes.h"));

        const bool bOk = FEnumJsonGenerator::GenerateAll(
            EnumRootDir,
            OutHeader,
            /*bRecurse=*/true,
            /*bSort=*/false,
            /*Underlying=*/TEXT("uint8"),
            /*Namespace=*/TEXT("")    // 원하면 "GameTypes" 등 지정
        );

        FNotificationInfo Info(bOk
            ? FText::Format(LOCTEXT("GenOK", "Generated: {0}"), FText::FromString(OutHeader))
            : FText::Format(LOCTEXT("GenFAIL", "Failed to generate: {0}"), FText::FromString(OutHeader)));

        Info.bFireAndForget = true;
        Info.FadeOutDuration = 0.5f;
        Info.ExpireDuration = 3.0f;
        auto N = FSlateNotificationManager::Get().AddNotification(Info);
        if (N.IsValid())
        {
            N->SetCompletionState(bOk ? SNotificationItem::CS_Success : SNotificationItem::CS_Fail);
        }

        // 필요하면 바로 파일 열기:
        // FSourceCodeNavigation::OpenSourceFile(OutHeader);
    }
};

IMPLEMENT_MODULE(FLegacyEditorModule, LegacyEditor);

#undef LOCTEXT_NAMESPACE