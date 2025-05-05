// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Legacy : ModuleRules
{
	public Legacy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"PhysicsCore"
		});

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "UMG",
            "Slate",
			"SlateCore",
            "Niagara",
            "MotionWarping",
            "AnimGraphRuntime"
        });
    }
}
