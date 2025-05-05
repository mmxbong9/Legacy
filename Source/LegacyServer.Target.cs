// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class LegacyServerTarget : TargetRules
{
	public LegacyServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;

		ExtraModuleNames.AddRange( new string[] { "Legacy" } );

		LegacyTarget.ApplySharedTargetSettings(this);
	}
}
