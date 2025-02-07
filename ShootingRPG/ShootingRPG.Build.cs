// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShootingRPG : ModuleRules
{
	public ShootingRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
	
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
	
	}
}
