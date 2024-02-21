// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Overwatch : ModuleRules
{
	public Overwatch(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "AnimGraphRuntime", "UMG" });
	}
}
