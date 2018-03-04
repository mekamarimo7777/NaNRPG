// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class NaNRPGEd : ModuleRules
{
	public NaNRPGEd(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"UnrealEd",
			"UMG",
			"Slate",
			"SlateCore",
			"EditorStyle",
			"AdvancedPreviewScene"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"ProceduralMeshComponent",
			"PropertyEditor",
			"AssetTools",
			"NaNRPG"
		});
		PrivateIncludePathModuleNames.AddRange(new string[] {
			"ProceduralMeshComponent",
			"UMG",
			"NaNRPG"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
		// if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
		// {
		//		if (UEBuildConfiguration.bCompileSteamOSS == true)
		//		{
		//			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
		//		}
		// }
	}
}
