// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LegacyTarget : TargetRules
{
	public LegacyTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "Legacy" } );
		ApplySharedTargetSettings(this);
	}

	internal static void ApplySharedTargetSettings(TargetRules Target)
	{
		Target.DefaultBuildSettings = BuildSettingsVersion.V5;
		Target.IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		Target.CppStandard = CppStandardVersion.Cpp20;
	}
}
