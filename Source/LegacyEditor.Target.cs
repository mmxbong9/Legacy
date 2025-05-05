// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LegacyEditorTarget : TargetRules
{
	public LegacyEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "Legacy", "LegacyEditor" } );

		LegacyTarget.ApplySharedTargetSettings(this);
	}
}
