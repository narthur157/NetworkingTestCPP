// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NetworkingTestCPP : ModuleRules
{
	public NetworkingTestCPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;


        PrivateIncludePaths.AddRange(
            new string[] {

                "NetworkingTestCPP/Private",
               
            }
        );


        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });






	}
}
