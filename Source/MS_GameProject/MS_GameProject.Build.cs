// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MS_GameProject : ModuleRules
{
	public MS_GameProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
