// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OnMyOwn : ModuleRules
{
	public OnMyOwn(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"OnMyOwn",
			"OnMyOwn/Variant_Platforming",
			"OnMyOwn/Variant_Platforming/Animation",
			"OnMyOwn/Variant_Combat",
			"OnMyOwn/Variant_Combat/AI",
			"OnMyOwn/Variant_Combat/Animation",
			"OnMyOwn/Variant_Combat/Gameplay",
			"OnMyOwn/Variant_Combat/Interfaces",
			"OnMyOwn/Variant_Combat/UI",
			"OnMyOwn/Variant_SideScrolling",
			"OnMyOwn/Variant_SideScrolling/AI",
			"OnMyOwn/Variant_SideScrolling/Gameplay",
			"OnMyOwn/Variant_SideScrolling/Interfaces",
			"OnMyOwn/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
