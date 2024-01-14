// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class PrimeiroGame : ModuleRules
{
	public PrimeiroGame(ReadOnlyTargetRules Target) : base(Target)
	{

		//para resolver problemas de compilação, principalmente quando existe muito arquivos .cpp e .h
		//resolver o problem de DWORD também, provavel que tem relação com o problema de cima
        bUseUnity = false;
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		//AnimGraphRuntime - for solve problem 'CommonAnimationTypes.generated.h': No such file or directory
		//NavigationSystem - for UNavigation need this

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "AIModule", "GameplayTasks", "AnimGraphRuntime", "NavigationSystem" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		//PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
