// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ArenaSurvivors : ModuleRules
{
    public ArenaSurvivors(ReadOnlyTargetRules Target) : base(Target)
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
            "Slate",
            "NavigationSystem"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[] {
            "ArenaSurvivors",
            "ArenaSurvivors/Variant_Platforming",
            "ArenaSurvivors/Variant_Platforming/Animation",
            "ArenaSurvivors/Variant_Combat",
            "ArenaSurvivors/Variant_Combat/AI",
            "ArenaSurvivors/Variant_Combat/Animation",
            "ArenaSurvivors/Variant_Combat/Gameplay",
            "ArenaSurvivors/Variant_Combat/Interfaces",
            "ArenaSurvivors/Variant_Combat/UI",
            "ArenaSurvivors/Variant_SideScrolling",
            "ArenaSurvivors/Variant_SideScrolling/AI",
            "ArenaSurvivors/Variant_SideScrolling/Gameplay",
            "ArenaSurvivors/Variant_SideScrolling/Interfaces",
            "ArenaSurvivors/Variant_SideScrolling/UI"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}