// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LegacyClientTarget : TargetRules
{
	public LegacyClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;
		
		ExtraModuleNames.AddRange( new string[] { "Legacy" } );

		LegacyTarget.ApplySharedTargetSettings(this);
	}
}
