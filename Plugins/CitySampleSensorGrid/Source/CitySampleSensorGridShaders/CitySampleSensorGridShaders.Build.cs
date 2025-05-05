// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CitySampleSensorGridShaders : ModuleRules
{
    public CitySampleSensorGridShaders(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "RenderCore",
            "RHI",
            "VectorVM",
            "Projects",
            "NiagaraCore",
            "NiagaraShader"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "CoreUObject",
            "Engine",
            "NiagaraCore"
        });

        if (Target.bBuildEditor)
        {
            PublicDependencyModuleNames.Add("TargetPlatform");
        }

        PrivateIncludePathModuleNames.AddRange(new string[]
        {
            "Niagara"
        });
    }
}