// Fill out your copyright notice in the Description page of Project Settings.

#include "LegacyEditor.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "LegacyEditor"
class FLegacyEditorModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
};

IMPLEMENT_MODULE(FLegacyEditorModule, LegacyEditor);

#undef LOCTEXT_NAMESPACE