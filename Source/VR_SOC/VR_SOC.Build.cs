// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class VR_SOC : ModuleRules
{
	public VR_SOC(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "OnlineSubsystem",  "OnlineSubsystemUtils", "RenderCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
	
		//Uncomment if you are using online features
		PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
