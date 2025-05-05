// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class LegacyEditor : ModuleRules
{
    public LegacyEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] {
            "LegacyEditor"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "Legacy"
        });

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] {
                "UnrealEd",
                "AnimationCore",
                "AnimGraph",
                "AnimGraphRuntime",
                "BlueprintGraph",
                "KismetCompiler",
                "EditorStyle",
                "GraphEditor",
                "PropertyEditor"
            });
        }
    }
}
